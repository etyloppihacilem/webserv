/* #############################################################################

               """          UploadStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 24 Jun. 2024 at 11:40
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "UploadStrategy.hpp"
#include "Body.hpp"
#include "BodyWriter.hpp"
#include "ClientRequest.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "Route.hpp"
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

template < class ServerClass, class RouteClass >
UploadStrategy< ServerClass, RouteClass >::UploadStrategy(
    ClientRequest     &request,
    const std::string &location,
    const ServerClass &server,
    bool               diff,
    size_t             max_size,
    bool               replace
) :
    ResponseBuildingStrategy(),
    _init(false),
    _server(server),
    _file(),
    _body(0),
    _target(request.get_target()),
    _location(location),
    _max_size(max_size),
    _replace(replace),
    _diff(diff) {
    (void) server;
    if (request.have_body()) {
        debug.log() << "Upload created with body" << std::endl;
        _body = request.get_body();
    } else
        debug.log() << "Upload created with no body" << std::endl;
}

template < class ServerClass, class RouteClass >
UploadStrategy< ServerClass, RouteClass >::~UploadStrategy() {
    if (_file.is_open())
        _file.close();
}

/*
  RFC 9110#section-9.3.3-4 :
  ===
  If one or more resources has been created on the origin server as a result of successfully processing a POST request,
  the origin server SHOULD send a 201 (Created) response containing a Location header field that provides an identifier
  for the primary resource created (Section 10.2.2) and a representation that describes the status of the request while
  referring to the new resource(s).
   */
template < class ServerClass, class RouteClass >
bool UploadStrategy< ServerClass, RouteClass >::build_response() {
    if (!_body) {
        if (!_init)
            init(); // creating file if not already there and init of headers and stuff
        if (_file.is_open())
            _file.close();
        _response.set_body("Empty (Success)");
        debug.log() << "No body to upload, upload done." << std::endl;
        return _built = true; // no body to upload
    }
    if (_built) {
        warn.log() << "UploadStrategy : trying to build response, but is already built." << std::endl;
        return _built;
    }
    if (!_init) {
        init();
        debug.log() << "Emptying buffer inside body." << std::endl;
        _file << _body->pop();
    } else if (!_body->is_done()) {
        size_t read = _body->read_body();
        debug.log() << "Read " << read << " bytes from socket to put in file." << std::endl;
        _file << _body->pop();
    }
    if (_body->length() > _max_size) {
        info.log() << "Max body size reached, sending " << ContentTooLarge << std::endl;
        throw HttpError(ContentTooLarge);
    }
    if (_body->is_done()) {
        _file.close();
        _response.set_body("Success");
        debug.log() << "Upload done." << std::endl;
        _done  = true;
        _built = true;
    }
    return _built;
}

template < class ServerClass, class RouteClass >
std::string UploadStrategy< ServerClass, RouteClass >::create_name(int nb) {
    std::stringstream st;
    st << "/uploadfile_" << nb;
    return st.str();
}

template < class ServerClass, class RouteClass >
void UploadStrategy< ServerClass, RouteClass >::init_location() {
    struct stat buf;
    bool        found = false;
    int         creat = 0;
    do {
        if (stat(_location.c_str(), &buf)) {
            if (errno == ENOENT) {
                debug.log() << "Upload location " << _location << " was not found and will be created." << std::endl;
                found = true; // we will create a file.
            }
            if (errno == EACCES) {
                warn.log() << "Could not open file " << _location << " for stat before upload, " << strerror(errno)
                           << ", sending " << Forbidden << std::endl;
                throw(HttpError(Forbidden));
            }
            if (errno == ENAMETOOLONG) {
                warn.log() << "Could not open file " << _location << " for stat before upload, " << strerror(errno)
                           << ", sending " << URITooLong << std::endl;
                throw(HttpError(URITooLong));
            }
            warn.log() << "Could not open file " << _location << " for stat before upload, " << strerror(errno)
                       << ", sending " << InternalServerError << std::endl;
            throw(HttpError(InternalServerError));
        } else {
            if (creat < 0) {
                error.log() << "New file creation did a int overflow, sending " << InternalServerError << std::endl;
                throw HttpError(InternalServerError);
            }
            if (creat) { // create in progress and file exists
                size_t last_slash = _location.find_last_of("/");
                if (last_slash == _location.npos) {
                    error.log() << "Error while incrementing new file count of " << _location << ", sending "
                                << InternalServerError << std::endl;
                    throw HttpError(InternalServerError);
                }
                _location.resize(last_slash);      // triming last name aka the one we just tested
                _location += create_name(creat++); // checking if uploadfile_[creat] exists there
                continue;                          // looping again
            }
            if (!S_ISDIR(buf.st_mode)) {
                debug.log() << "Found file " << _location << " for upload." << std::endl;
                found = true; // file is a file and is found
            } else {          // this is a directory
                debug.log() << "Location " << _location << " for upload is a directory, creating a new file."
                            << std::endl;
                _location += create_name(creat++); // checking if uploadfile_0 exists there
            }
        }
    } while (!found);
}

template < class ServerClass, class RouteClass >
void UploadStrategy< ServerClass, RouteClass >::init() {
    if (_init) {
        warn.log() << "Init is already done and does not need to be done again." << std::endl;
        return;
    }
    _file.open(_location.c_str(), std::fstream::in);
    if (_diff) {
        std::string location = _server.getUploadLocation(_location);
        if (location != "")
            _response.add_header("Location", location);
    } else
        _response.add_header("Location", _target);
    if (!_file.is_open() || _replace) // check difference of status code if file does not exists yet
        _response.set_code(Created);
    else
        _response.set_code(OK);
    _file.close();
    _file.open(_location.c_str(), std::fstream::out | (_replace ? std::fstream::trunc : std::fstream::app));
    if (!_file.is_open()) {
        if (errno == ENOENT) { // cause could be validated with stat if component of path does not exists
            info.log() << "UploadStrategy: could not open file " << _location << ", " << strerror(errno) << ". Sending "
                       << NotFound << std::endl;
            throw HttpError(NotFound);
        }
        error.log() << "UploadStrategy: cannot open " << _location << " to upload: " << strerror(errno) << ". Sending "
                    << Forbidden << std::endl;
        throw HttpError(Forbidden);
    }
    debug.log() << "Successfully opened file " << _target << std::endl;
    _init = true;
}

template < class ServerClass, class RouteClass >
bool UploadStrategy< ServerClass, RouteClass >::fill_buffer(std::string &buffer, size_t size) {
    (void) buffer;
    (void) size;
    warn.log() << "fill_buffer called in an object with no body (UploadStrategy)." << std::endl;
    return _done = true;
}

template class UploadStrategy<>;

#ifdef TESTING
# include "FakeRoute.hpp"
# include "FakeServer.hpp"
template class UploadStrategy< FakeServer, FakeRoute >; // force compilation for test templates
#endif
