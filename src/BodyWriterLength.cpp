/* #############################################################################

               """          BodyWriterLength.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 10:54
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyWriterLength.hpp"
#include "BodyWriter.hpp"
#include "Logger.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "StringUtils.hpp"
#include "todo.hpp"
#include <cstddef>
#include <new>
#include <ostream>
#include <stdint.h>
#include <string>

BodyWriterLength::BodyWriterLength(const std::string &body_content) : BodyWriter(), _body(body_content) {}

BodyWriterLength::BodyWriterLength(ResponseBuildingStrategy &state) : BodyWriter(state), _body() {
    size_t i = 0;

    while (++i <= MAX_BODY_BUFFER && !(_strategy->fill_buffer(_body)) && length() <= MAX_BODY_BUFFER)
        ; // i is to prevent infinite loop in case buffer is not filling
    if (length() > MAX_BODY_BUFFER) {
        _body = "";
        shrink_to_fit(_body); // wiping body;
        throw std::bad_alloc();
    }
    if (i > MAX_BODY_BUFFER)
        error.log() << "BodyWriterLength : maximum fill_buffer iterations exceeded. This happens to prevent "
                    << "infinite loop. " << "Result may be incomplete. Check value of MAX_BODY_BUFFER." << std::endl;
}

BodyWriterLength::~BodyWriterLength() {}

/**
  Return whole body.
  */
std::string BodyWriterLength::generate(size_t size /** is discarded because not needed in BodyWriterLength */) {
    if (_done)
        warn.log() << "Body already recovered. Risk of loosing body data if save_mem procedures are initiated."
                   << std::endl;
    if (size > _body.length())
        _done = true;
    std::string ret = _body.substr(0, (size > _body.length() ? _body.length() : size));
    _body.replace(0, (size > _body.length() ? _body.length() : size), "");
    _length += ret.length();
    return ret;
}

size_t BodyWriterLength::length() const {
    return _length + _body.length();
}

void BodyWriterLength::save_mem() {
    if (_done)
        _body = "";
    shrink_to_fit(_body); // reducing memory usage.
}
