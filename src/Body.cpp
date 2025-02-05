/* #############################################################################

               """          Body.cpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:59
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Body.hpp"
#include "StringUtils.hpp"
#include <cstddef>
#include <string>

Body::Body(int socket, std::string &buffer) :
    _socket(socket),
    _buffer(buffer),
    _done(false),
    _uniform(true),
    _body(),
    _total(0) {}

Body::~Body() {}

bool Body::is_done() const {
    return _done;
}

size_t Body::length() const {
    return _total;
}

void Body::save_mem() {
    if (_uniform)
        _body = "";
    shrink_to_fit(_buffer);
    shrink_to_fit(_body);
}
