/* #############################################################################

               """          Messages.cpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 17:51
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Messages.hpp"

Messages::Messages() {}

Messages::~Messages() {}

Messages::Messages(const Messages &other) {
    (void) other;
}

Messages &Messages::operator=(const Messages &other) {
    if (&other == this)
        return (*this);
    return (*this);
}
