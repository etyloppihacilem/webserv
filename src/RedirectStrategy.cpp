/* #############################################################################

               """          RedirectStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 12 Jun. 2024 at 10:22
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "RedirectStrategy.hpp"
#include "ResponseBuildingStrategy.hpp"

RedirectStrategy::RedirectStrategy(const std::string &location, ResponseBuildState &state):
    ResponseBuildingStrategy(state),
    _location               (location) {}

RedirectStrategy::~RedirectStrategy() {}

void RedirectStrategy::buildResponse() {
    ;
}

bool RedirectStrategy::fill_buffer(std::string &buffer, size_t size) {
    (void) buffer;
    (void) size;
    return (false);
}
