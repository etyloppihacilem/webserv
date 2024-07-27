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
#include <cstddef>
#include <fstream>
#include <ostream>
#include <string>
#include <unistd.h>

UploadStrategy::UploadStrategy(ClientRequest &request, const std::string &location, bool replace):
    ResponseBuildingStrategy(),
    _init                   (false),
    _file                   (),
    _body                   (0),
    _target                 (request.get_target()),
    _location               (location),
    _replace                (replace) {
    if (request.have_body())
        _body = request.get_body();
}

UploadStrategy::~UploadStrategy() {
    if (_file.is_open())
        _file.close();
}

bool UploadStrategy::build_response() {
    if (!_body && !_init) {
        init(); // creating file if not already there and init of headers and stuff
        _file.close();
        return _built = true; // no body
    }
    if (_built) {
        warn.log() << "UploadStrategy : trying to build response, but is already built." << std::endl;
        return _built;
    }
    if (!_init)
        init();
    else if (!_body->is_done())
        _file << _body->pop();
    else {
        _file.close();
        _done   = true;
        _built  = true;
    }
    return _built;
}

void UploadStrategy::init() {
    if (_init) {
        warn.log() << "Init is already done and does not need to be done again." << std::endl;
        return;
    }
    _file.open(_location.c_str(), std::fstream::in);
    _response.add_header("Location", _target);
    if (!_file.is_open() || _replace) // check difference of status code if file does not exists yet
        _response.set_code(Created);
    else
        _response.set_code(OK);
    _file.close();
    _file.open(_location.c_str(), std::fstream::out | (_replace ? std::fstream::trunc : std::fstream::app));
    if (!_file.is_open()) {
        error.log() << "Cannot open " << _location << " to upload. Sending " << Forbidden << std::endl;
        throw HttpError(Forbidden);
    }
    _init = true;
}

bool UploadStrategy::fill_buffer(std::string &buffer, size_t size) {
    (void) buffer;
    (void) size;
    warn.log() << "fill_buffer called in an object with no body (UploadStrategy)." << std::endl;
    return _done = true;
}
