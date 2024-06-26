/* #############################################################################

               """          CGIStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 26 Jun. 2024 at 11:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "CGIStrategy.hpp"
#include "ResponseBuildState.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "StringUtils.hpp"
#include <cstddef>
#include <string>

CGIStrategy::CGIStrategy(ResponseBuildState &state):
    ResponseBuildingStrategy(state) {}

CGIStrategy::~CGIStrategy() {}

bool CGIStrategy::build_response() {
    return _built;
}

bool CGIStrategy::fill_buffer(std::string &buffer, size_t size) {
    return _done;
}

void CGIStrategy::save_mem() {
    shrink_to_fit(_buffer);
    _response.save_mem();
}
