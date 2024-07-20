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

UploadStrategy::UploadStrategy(ClientRequest &request, std::string location, bool replace):
    ResponseBuildingStrategy(),
    _request                (request),
    _location               (location),
    _replace                (replace) {}

UploadStrategy::~UploadStrategy() {}

// OPTI:reading incoming body is blocking the server atm. think about doing it in a virtual loop !!!
bool UploadStrategy::build_response() {
    if (_built) {
        warn.log() << "UploadStrategy : trying to build response, but is already built." << std::endl;
        return _built;
    }
    if (!_request.have_body()) {
        error.log() << "Upload strategy but request have no body. Sending " << InternalServerError << std::endl;
        throw HttpError(InternalServerError);
    }

    Body            *body = _request.get_body();
    std::fstream    file;

    file.open(_location.c_str(), std::fstream::in);
    if (!file.is_open() || _replace) // check difference of status code if file does not exists yet
        _response.set_code(Created);
    else
        _response.set_code(OK);
    file.close();
    file.open(_location.c_str(), std::fstream::out | (_replace ? std::fstream::trunc : std::fstream::app));
    if (!file.is_open()) {
        error.log() << "Cannot open " << _location << " to upload. Sending " << Forbidden << std::endl;
        throw HttpError(Forbidden);
    }
    while (!body->is_done()) {
        file << body->pop();
    }
    file.close();
    _response.add_header("Location", _location);
    _done           = true;
    return _built   = true;
}

bool UploadStrategy::fill_buffer(std::string &buffer, size_t size) {
    (void) buffer;
    (void) size;
    warn.log() << "fill_buffer called in an object with no body (UploadStrategy)." << std::endl;
    return _done = true;
}
