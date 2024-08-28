/* #############################################################################

               """          BodyLength.cpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:52
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyLength.hpp"
#include "Body.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ServerConfFields.hpp"
#include "todo.hpp"
#include <cctype>
#include <cstddef>
#include <ostream>
#include <sstream>
#include <string>
#include <unistd.h>

BodyLength::BodyLength(int socket, std::string &buffer, std::string length) :
    Body(socket, buffer),
    _length(0),
    _read_length(_buffer.length()) {
    for (std::string::iterator i = length.begin(); i != length.end(); i++)
        if (!isdigit(*i)) {
            info.log() << "BodyLength: Invalid length detected (NaN), sending " << BadRequest << std::endl;
            throw HttpError(BadRequest); // invalid length
        }

    std::stringstream tmp(length);

    if (!(tmp >> _length)) {
        info.log() << "BodyLength: Converting length '" << length << "' to number failed, sending " << BadRequest << std::endl;
        throw HttpError(BadRequest); // other invalid length
    }
    _total = _length;
    if (_total > MAX_BODY_SIZE){
        info.log() << "Body is too large (" << _total << " bytes), sending " << ContentTooLarge << std::endl;
        throw HttpError(ContentTooLarge);
    }
}

BodyLength::~BodyLength() {}

/**
  Function used to fill _buffer. Will read the body until last byte and not more.
  */
size_t BodyLength::read_body() {
    if (_done)
        return 0;

    char   buf[BUFFER_SIZE + 1]; // the whole buffer is set to 0
    size_t size_read;

    for (size_t i = 0; i < BUFFER_SIZE + 1; i++)
        buf[i] = 0;
    size_read     = read(_socket, buf, (_length - _read_length >= BUFFER_SIZE) ? BUFFER_SIZE : _length - _read_length);
    _buffer.insert(_buffer.length(), buf);
    _read_length += size_read;
    if (_length <= _read_length)
        _done = true;
    return size_read;
}

std::string &BodyLength::get() {
    read_body();
    _body  += _buffer;
    _buffer = "";
    return _body;
}

std::string BodyLength::pop() {
    std::string tmp = _body;

    _uniform = false;
    _body    = "";
    read_body();
    tmp     += _buffer;
    _buffer  = "";
    return tmp;
}

void BodyLength::clean() {
    pop();
}
