/* #############################################################################

               """          BodyChunk.cpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:52
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyChunk.hpp"
#include "Body.hpp"
#include "Logger.hpp"
#include "todo.hpp"
#include <cctype>
#include <cstddef>
#include <ios>
#include <iterator>
#include <sstream>
#include <unistd.h>

BodyChunk::BodyChunk(int fd, std::string &buffer):
    Body            (fd, buffer),
    _bytes_remaining(0) {}

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
    return (size_read);
}

std::string &BodyChunk::get() {
    return (_buffer);
}

std::string BodyChunk::pop() {
    return ("");
}

// returns true if a new chunk is found and updates _bytes remaining
// does nothing if _bytes_remaining is not 0
bool BodyChunk::init_chunk() { // discard until a size line is found
    size_t                  sp;
    std::string::iterator   it = _buffer.begin();
    std::string::iterator   i;

    if (_bytes_remaining > 0)
        return (false);
    while (it != _buffer.end()) {
        for (i = it; i != _buffer.end() && is_hex(*i); i++)
            ;                       // nothing
        if (i == _buffer.end())
            return (false);
        if (i - it > 0) {

            std::stringstream tmp;  // there is the length were looking for

            tmp << std::hex << _buffer.substr(0, std::distance(_buffer.begin(), i));
            tmp >> _bytes_remaining;
            sp      = _buffer.find("\r\n", 0);
            _buffer = _buffer.substr(sp + 2, _buffer.length() - (sp + 2));
            return (true);
        } else {
            sp = _buffer.find("\r\n", 0);
            if (sp == _buffer.npos) {
                _buffer = "";
                return (false);
            }
            _buffer = _buffer.substr(sp + 2, _buffer.length() - (sp + 2));
        }
        it = _buffer.begin();
    }
    return (false);
}

bool BodyChunk::is_hex(int c) {
    if (std::isdigit(c))
        return (true);
    if (('a' <= c && c <= 'f') || ('A' <= c && c <= 'F'))
        return (true);
    return (false);
}
