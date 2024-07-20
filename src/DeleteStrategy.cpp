/* #############################################################################

               """          DeleteStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 25 Jun. 2024 at 12:22
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "DeleteStrategy.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "StringUtils.hpp"
#include <cerrno>
#include <cstdio>
#include <ostream>
#include <string>

DeleteStrategy::DeleteStrategy(const std::string &location):
    ResponseBuildingStrategy(),
    _location               (location) {}

DeleteStrategy::~DeleteStrategy() {}

/**
  Where the deletion really happens.
  */
bool DeleteStrategy::build_response() {
    if (_built) {
        warn.log() << "DeleteStrategy : trying to build response, but is already built." << std::endl;
        return _built;
    }
    if (std::remove(_location.c_str())) {
        if (errno == ENOENT)
            throw HttpError(NotFound);
        if (errno == EACCES)
            throw HttpError(Forbidden);
        error.log() << "Unknown error while deleting " << _location << ", throwing " << InternalServerError
                    << std::endl;
        throw HttpError(InternalServerError);
    }
    _response.set_code(NoContent); // 204, everything is done and there's no content
    _done           = true;
    return _built   = true;
}

bool DeleteStrategy::fill_buffer(std::string &buffer, size_t size) {
    (void) buffer;
    (void) size;
    warn.log() << "fill_buffer called in an object with no body (DeleteStrategy)." << std::endl;
    return _done = true;
}

void DeleteStrategy::save_mem() {
    shrink_to_fit(  _location);
    shrink_to_fit(  _buffer);
    _response.save_mem();
}
