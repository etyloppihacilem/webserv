/* #############################################################################

               """          ResponseBuildingStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:39
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ResponseBuildingStrategy.hpp"
#include "Response.hpp"
#include <cstddef>

ResponseBuildingStrategy::ResponseBuildingStrategy(ResponseBuildState &state):
    _state          (&state),
    _response       (),
    _done           (false),
    _estimated_size (0) {
    (void) _state; // TODO delete this
}

ResponseBuildingStrategy::~ResponseBuildingStrategy() {}

Response &ResponseBuildingStrategy::get_response() {
    return _response;
}

bool ResponseBuildingStrategy::is_done() const {
    return _done;
}

size_t ResponseBuildingStrategy::get_estimated_size() const {
    return _estimated_size;
}
