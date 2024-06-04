/* #############################################################################

               """          Body.cpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:59
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Body.hpp"
#include "todo.hpp"
#include <unistd.h>

Body::Body(int fd, std::string &buffer):
    _fd     (fd),
    _buffer (buffer),
    _done   (false),
    _body   () {}

Body::~Body() {}

bool Body::is_done() const {
    return (_done);
}
