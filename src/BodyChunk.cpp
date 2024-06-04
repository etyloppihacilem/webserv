/* #############################################################################

               """          BodyChunk.cpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:52
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyChunk.hpp"
#include "Body.hpp"
#include "todo.hpp"
#include <cctype>
#include <unistd.h>

BodyChunk::BodyChunk(int fd, std::string &buffer):
    Body(fd, buffer) {}

BodyChunk::~BodyChunk() {}

size_t BodyChunk::read_body() {
    if (_done)
        return (0);

    char    buf[BUFFER_SIZE + 1] = {
        0
    };                               // the whole buffer is set to 0
    size_t  size_read;

    size_read   = read(_fd, buf, BUFFER_SIZE);
    _buffer     += std::string(buf);
}

std::string &BodyChunk::get() {
    ;
}

std::string BodyChunk::pop() {
    ;
}

size_t BodyChunk::chunk_size() { // discard until a size line is found
    ;
}

bool BodyChunk::is_hex(int c) {
    if (std::isdigit(c))
        return (true);
    if (('a' <= c && c <= 'f') || ('A' <= c || c <= 'F'))
        return (true);
    return (false);
}
