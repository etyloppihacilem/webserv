/* #############################################################################

               """          GetIndexStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 14:11
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "GetIndexStrategy.hpp"
#include "BodyWriter.hpp"
#include <cstddef>
#include <string>
#include <dirent.h>

GetIndexStrategy::GetIndexStrategy(const std::string &location, ResponseBuildState &state): ResponseBuildingStrategy(state) {}

GetIndexStrategy::~GetIndexStrategy() {}

bool GetIndexStrategy::fill_buffer(std::string &buffer, size_t size) {
    DIR *dir = opendir(_location.c_str());
    
}

void GetIndexStrategy::buildResponse() {
    ;
}
