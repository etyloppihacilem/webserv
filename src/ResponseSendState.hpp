/* #####################################################################################################################

               """          ResponseSendState.hpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 14:31
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_SRC_RESPONSESENDSTATE_HPP_
#define INCLUDE_SRC_RESPONSESENDSTATE_HPP_

#include "ProcessState.hpp"

class ResponseSendState : public ProcessState {
    public:
        ResponseSendState(int socket);
        ~ResponseSendState();

    private:
};

#endif // INCLUDE_SRC_RESPONSESENDSTATE_HPP_
