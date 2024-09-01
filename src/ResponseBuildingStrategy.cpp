/* #############################################################################

               """          ResponseBuildingStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:39
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ResponseBuildingStrategy.hpp"
#include "Logger.hpp"
#include "Response.hpp"
#include <cstddef>
#include <ostream>

ResponseBuildingStrategy::ResponseBuildingStrategy(bool is_head) :
    _response(),
    _built(false),
    _done(false),
    _estimated_size(0),
    _is_head(is_head) {
    _response.set_head(_is_head);
}

ResponseBuildingStrategy::~ResponseBuildingStrategy() {
    debug.log() << "Strategy is deleting response." << std::endl;
}

/**
  To get the response. _response can only be used when is_done() returns true.
  */
Response &ResponseBuildingStrategy::get_response() {
    return _response;
}

/**
  Returns true if response generating is done.
  */
bool ResponseBuildingStrategy::is_done() const {
    return _done;
}

/**
  Returns true if response building is done.
  */
bool ResponseBuildingStrategy::is_built() const {
    return _built;
}

/**
  Returns estimated size. Is mostly used to choose which BodyWriter type is going to be built.
  This is approximate because in most case, the whole content have not been read yet to save on performances.
  */
size_t ResponseBuildingStrategy::get_estimated_size() const {
    return _estimated_size;
}

/**
  This function is used to save as most memory as possible in case more heap is needed. This is not at every actions
  because of performances issues in normal operations.
  */
void ResponseBuildingStrategy::save_mem() {
    _response.save_mem();
}
