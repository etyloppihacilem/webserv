/* #############################################################################

               """          BodyWriterChunk.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 10:52
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyWriterChunk.hpp"
#include "ResponseBuildingStrategy.hpp"
#include <cstddef>
#include <string>

BodyWriterChunk::BodyWriterChunk(ResponseBuildingStrategy &state):
    BodyWriter(state) {
    _strategy->buildResponse();
}

BodyWriterChunk::~BodyWriterChunk() {}

std::string BodyWriterChunk::generate(size_t size) {
    if (_done)
        return ("");

    std::string temp;

    _done = _strategy->fill_buffer(temp, size);
    return (temp);
}
