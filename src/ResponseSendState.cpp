/* #####################################################################################################################

               """          ResponseSendState.cpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 14:31
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ResponseSendState.hpp"
#include "ProcessState.hpp"

ResponseSendState::ResponseSendState(int socket) : ProcessState(socket) {}

ResponseSendState::~ResponseSendState() {}
