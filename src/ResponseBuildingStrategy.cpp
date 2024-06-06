/* #############################################################################

               """          ResponseBuildingStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:39
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ResponseBuildingStrategy.hpp"
#include "Response.hpp"

ResponseBuildingStrategy::ResponseBuildingStrategy(ResponseBuildState &state):
    _state(state) {}

ResponseBuildingStrategy::~ResponseBuildingStrategy() {}

Response &ResponseBuildingStrategy::get_response() {
    return (_response);
}

bool ResponseBuildingStrategy::is_done() {
    return (_done);
}
