/* #############################################################################

               """          UploadStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 24 Jun. 2024 at 11:40
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "UploadStrategy.hpp"
#include "BodyWriter.hpp"
#include "ResponseBuildState.hpp"
#include <string>

UploadStrategy::UploadStrategy(ResponseBuildState &state, std::string location):
    ResponseBuildingStrategy(state),
    _location               (location) {}

UploadStrategy::~UploadStrategy() {}
