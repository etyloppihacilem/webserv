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
    BodyWriter  (state),
    _length     (0) {
    _strategy->buildResponse();
}

BodyWriterChunk::~BodyWriterChunk() {}

/**
  Returns string of maximum size, of body.
  */
std::string BodyWriterChunk::generate(size_t size) {
    if (_done)
        return "";

    std::string temp;

    _done   = _strategy->fill_buffer(temp, size);
    _length += temp.length();
    return temp;
}

size_t BodyWriterChunk::length() const {
    return _length;
}

void BodyWriterChunk::save_mem() {
    ;
}
