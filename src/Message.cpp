/* #############################################################################

               """          Message.cpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 17:51
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Message.hpp"

Message::Message() {}

Message::~Message() {}

Message::Message(const Message &other) {
    (void) other;
}

Message &Message::operator=(const Message &other) {
    if (&other == this)
        return (*this);
    return (*this);
}
