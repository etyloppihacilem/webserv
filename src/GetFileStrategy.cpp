/* #############################################################################

               """          GetFileStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 19 Jun. 2024 at 11:25
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "GetFileStrategy.hpp"
#include "BodyWriter.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "MimeTypes.hpp"
#include "StringUtils.hpp"
#include "todo.hpp"
#include <cerrno>
#include <cstddef>
#include <fstream>
#include <ios>
#include <new>
#include <ostream>
#include <string>
#include <strings.h>
#include <sys/stat.h>

GetFileStrategy::GetFileStrategy(const MimeTypes &mime, const std::string &location, HttpCode code) :
    ResponseBuildingStrategy(),
    _mime(mime),
    _location(location),
    _code(code) {}

GetFileStrategy::~GetFileStrategy() {
    if (_file.is_open())
        _file.close();
}

bool GetFileStrategy::build_response() {
    if (_built) {
        warn.log() << "GetFileStrategy : trying to build response, but is already built." << std::endl;
        return _built;
    }
    if (_file.is_open()) {
        warn.log() << "File " << _location << " was already opened. Closing and reopening." << std::endl;
        _file.close();
    }
    {
        struct stat buf;

        if (stat(_location.c_str(), &buf)) {
            if (errno == ENOENT)
                throw HttpError(NotFound);
            if (errno == EACCES)
                throw HttpError(Forbidden);
            if (errno == ENOMEM)
                throw std::bad_alloc();
            if (errno == ENOTDIR)
                throw HttpError(Forbidden, "DIR"); // this is to tell the difference between directory forbidden and no
                                                   // directory forbidden
            if (errno == ENAMETOOLONG)
                throw HttpError(URITooLong);
            throw HttpError(InternalServerError);
        }
        _estimated_size = buf.st_size;
    } // saving stack
    _file.open(_location.c_str(), std::fstream::binary | std::ios_base::in);
    if (!_file.is_open()) {
        error.log() << "Undetected error opening file " << _location << ". Sending " << InternalServerError
                    << std::endl;
        throw HttpError(InternalServerError);
    }
    _response.set_body(*this);

    std::string extension = extract_extension(_location);

    _response.set_code(_code);
    _response.add_header("Content-Type", _mime.get_type(extension));
    if (!_mime.has_type(extension))
        _response.add_header("Content-Disposition", "attachment; filename=\"" + extract_basename(_location) + "\"");
    //< this is meant to download file in case the mime type is not known.
    return _built = true;
}

bool GetFileStrategy::fill_buffer(std::string &buffer, size_t size) {
    char buf[BUFFER_SIZE + 1] = {0}; // init with 0

    if (!_file.is_open()) {
        error.log() << "File " << _location << " is read but not open." << std::endl;
        throw HttpError(InternalServerError);
    }
    if (_done)
        return _done;
    _file.read(buf, size > BUFFER_SIZE ? BUFFER_SIZE : size);
    if (_file.eof())
        _done = true;
    buffer += buf;
    return _done;
}

void GetFileStrategy::save_mem() {
    shrink_to_fit(_location);
}
