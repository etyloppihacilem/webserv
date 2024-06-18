/* #############################################################################

               """          Body.cpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:59
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Body.hpp"
#include "StringUtils.hpp"
#include "todo.hpp"
#include <cstddef>
#include <unistd.h>

Body::Body(int fd, std::string &buffer):
    _fd     (fd),
    _buffer (buffer),
    _done   (false),
    _uniform(true),
    _body   () {}

Body::~Body() {}

bool Body::is_done() const {
    return _done;
}

size_t Body::length() const {
    return _total;
}

void Body::save_mem() {
    shrink_to_fit(_buffer);
    shrink_to_fit(_body);
}
