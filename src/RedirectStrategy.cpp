/* #############################################################################

               """          RedirectStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 12 Jun. 2024 at 10:22
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "RedirectStrategy.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ResponseBuildingStrategy.hpp"

RedirectStrategy::RedirectStrategy(const std::string &location, ResponseBuildState &state):
    ResponseBuildingStrategy(state),
    _location               (location) {}

RedirectStrategy::~RedirectStrategy() {}

void RedirectStrategy::buildResponse() {
    _response.add_header("Location", _location);
    _response.set_code(MovedPermanently);
}

bool RedirectStrategy::fill_buffer(std::string &buffer, size_t size) {
    (void) buffer;
    (void) size;
    warn.log("fill_buffer called in an object with no body (RedirectStrategy).");
    return true; // body is not supposed to even exists
}
