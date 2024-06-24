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
#include <cctype>
#include <cstddef>
#include <ios>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include "todo.hpp"

BodyChunk::BodyChunk(int fd, std::string &buffer):
    Body            (fd, buffer),
    _bytes_remaining(0),
    _trailing       (false) {}

BodyChunk::~BodyChunk() {}

/**
  This function will read from the socket and happen the result to _buffer to be processed.
  */
size_t BodyChunk::read_body() {
    if (_done)
        return 0;

    char    buf[BUFFER_SIZE + 1] = {
        0
    };                                          // whole buffer is set to 0
    size_t  size_read;

    size_read = read(_fd, buf,
            (_bytes_remaining ? _bytes_remaining : 1)
            > BUFFER_SIZE ? BUFFER_SIZE : (_bytes_remaining ? _bytes_remaining : 1));
    _buffer += std::string(buf);
    return size_read;
}

std::string &BodyChunk::get() {
    if (_done || !_uniform)
        return _body;

    size_t i = 0;

    while (!_bytes_remaining && i <= BUFFER_SIZE) {
        read_body();
        init_chunk();
        i++;
    }
    if (i > BUFFER_SIZE)
        error.log() << "Reading body error" << std::endl;
    if (_bytes_remaining)
        read_body();
    if (_bytes_remaining) {
        size_t to_save = (_bytes_remaining > _buffer.length() ? _buffer.length() : _bytes_remaining);

        _body               += _buffer.substr(0, to_save);
        _buffer             = _buffer.substr(to_save, _buffer.length() - to_save);
        _bytes_remaining    -= to_save;
        _total              += to_save;
    }
    return _body;
}

std::string BodyChunk::pop() {
    if (_done)
        return _body;

    std::string ret = "";
    size_t      i   = 0;

    _uniform = false;
    while (!_bytes_remaining && i <= BUFFER_SIZE) {
        read_body();
        init_chunk();
        i++;
    }
    if (i > BUFFER_SIZE)
        error.log() << "Reading body error" << std::endl;
    if (_bytes_remaining)
        read_body();
    if (_bytes_remaining) {
        size_t to_save = (_bytes_remaining > _buffer.length() ? _buffer.length() : _bytes_remaining);

        ret                 = _buffer.substr(0, to_save);
        _buffer             = _buffer.substr(to_save, _buffer.length() - to_save);
        _bytes_remaining    -= to_save;
        _total              += to_save;
    }
    return ret;
}

void BodyChunk::clean() {
    while (!_done)
        pop();
}

/**
  This functions looks for the hex size of a chunk to initiate its reading.

  Calling init_chunk() before the beginning of the chunk will discard any bytes found before the first hex size.

  Calling init_chunk() if a chunk is currently being read (aka if _bytes_remaining != 0) will not do anything and
  triggers a warning.
  */
bool BodyChunk::init_chunk() { // discard until a size line is found
    size_t                  sp;
    std::string::iterator   it = _buffer.begin();
    std::string::iterator   i;

    if (_trailing) {
        sp = _buffer.find("\r\n", 0);
        if (sp == _buffer.npos)
            _trailing = true;
        else {
            _buffer     = _buffer.substr(sp + 2, _buffer.length() - (sp + 2));          // skipping trailer section
            _trailing   = false;
            _done       = true;
        }
    }
    if (_bytes_remaining > 0 || _done) {
        warn.log() << "Trying to initiate a chunk while another is still being read." << std::endl;
        return false;
    }
    while (it != _buffer.end()) {
        for (i = it; i != _buffer.end() && is_hex(*i); i++)
            ; // nothing
        if (i == _buffer.end())
            return false;
        if (i - it > 0) {
            sp = _buffer.find("\r\n", 0);
            if (sp == _buffer.npos)
                return false;

            std::stringstream tmp;

            tmp << std::hex << _buffer.substr(0, std::distance(_buffer.begin(), i));
            tmp >> _bytes_remaining;
            _buffer = _buffer.substr(sp + 2, _buffer.length() - (sp + 2));
            if (!_bytes_remaining) { // if last byte
                _done   = true;
                sp      = _buffer.find("\r\n", 0);
                if (sp == _buffer.npos) {
                    _trailing   = true;
                    _done       = false;
                } else
                    _buffer = _buffer.substr(sp + 2, _buffer.length() - (sp + 2)); // skipping trailer section
                return false;
            }
            return true;
        } else {
            sp = _buffer.find("\r\n", 0);
            if (sp == _buffer.npos) {
                _buffer = "";
                return false;
            }
            _buffer = _buffer.substr(sp + 2, _buffer.length() - (sp + 2));
        }
        it = _buffer.begin();
    }
    return false;
}

/// Returns true if c is a hex character
bool BodyChunk::is_hex(int c) {
    if (std::isdigit(c))
        return true;
    if (('a' <= c && c <= 'f') || ('A' <= c && c <= 'F'))
        return true;
    return false;
}
