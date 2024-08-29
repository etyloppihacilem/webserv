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
#include "ResponseBuildingStrategy.hpp"
#include <string>

class ResponseSendState : public ProcessState {
    public:
        ResponseSendState(int socket, ResponseBuildingStrategy *strategy);
        ~ResponseSendState();

        t_state process();
        void    save_mem();

    private:
        ResponseBuildingStrategy *_strategy;
        std::string               _buffer;
        bool                      _sent;
};

#endif // INCLUDE_SRC_RESPONSESENDSTATE_HPP_
