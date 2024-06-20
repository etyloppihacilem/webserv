/* #############################################################################

               """          BodyWriterLength.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 10:54
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyWriterLength.hpp"
#include "Logger.hpp"
#include "StringUtils.hpp"
#include "todo.hpp"
#include <stdint.h>
#include <string>

BodyWriterLength::BodyWriterLength(ResponseBuildingStrategy &state):
    BodyWriter  (state),
    _body       (),
    _recovered  (false) {
    uint16_t i = 0;

    while (!(_done = _strategy->fill_buffer(_body)) && ++i && length() < MAX_BODY_BUFFER)
        ;                       // i is to prevent infinite loop in case buffer is not filling
    if (length() < MAX_BODY_BUFFER) {
        _body = "";
        shrink_to_fit(_body);   // wiping body;
        throw std::bad_alloc();
    }
    if (i == 0)
        error.log("BodyWriterLength : maximum fill_buffer iterations exceeded. This happens to prevent infinite loop. "
                  "Result may be incomplete. Check value of MAX_BODY_BUFFER.");
    _done = true;
}

BodyWriterLength::~BodyWriterLength() {}

/**
  Return whole body.
  */
std::string BodyWriterLength::generate(size_t size /** is discarded because not needed in BodyWriterLength*/) {
    (void) size;
    if (_recovered)
        warn.log("Body already recovered. Risk of loosing body data if save_mem procedures are initiated.");
    _recovered = true;
    return _body;
}

size_t BodyWriterLength::length() const {
    return _body.length();
}

void BodyWriterLength::save_mem() {
    if (_recovered)
        _body = "";
    shrink_to_fit(_body); // reducing memory usage.
}
