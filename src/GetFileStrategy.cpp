/* #############################################################################

               """          GetFileStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 19 Jun. 2024 at 11:25
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "GetFileStrategy.hpp"
#include "BodyWriter.hpp"
#include <string>

GetFileStrategy::GetFileStrategy(std::string &location, ResponseBuildingStrategy &state):
    ResponseBuildingStrategy(state),
    _location               (location) {}

GetFileStrategy::~GetFileStrategy() {}

void GetFileStrategy::buildResponse() {
    ;
}

bool GetFileStrategy::fill_buffer() {
    ;
}

void GetFileStrategy::save_mem() {
    ;
}
