/* #############################################################################

               """          BodyWriterChunk.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 10:52
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyWriterChunk.hpp"
#include "BodyWriter.hpp"
#include "Logger.hpp"
#include "ResponseBuildingStrategy.hpp"
#include <cstddef>
#include <ios>
#include <ostream>
#include <sstream>
#include <string>

BodyWriterChunk::BodyWriterChunk(ResponseBuildingStrategy &strategy) : BodyWriter(strategy) {}

BodyWriterChunk::~BodyWriterChunk() {}

/**
  Returns string of maximum size, of body.
  */
std::string BodyWriterChunk::generate(size_t size) {
    if (_done) {
        warn.log() << "BodyWriterChunk called when body is done generating." << std::endl;
        return "";
    }

    std::string temp;
    _done = _strategy->fill_buffer(temp, size);
    std::stringstream st;
    st << std::hex << temp.size();
    _length += temp.length();
    temp     = st.str() + "\r\n" + temp + (_done ? "\r\n0\r\n\r\n" : "\r\n");
    return temp;
}

size_t BodyWriterChunk::length() const {
    return _length;
}

void BodyWriterChunk::save_mem() {
    ;
}
