/* #####################################################################################################################

               """          ProcessHandler.hpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 14:33
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_SRC_PROCESSHANDLER_HPP_
#define INCLUDE_SRC_PROCESSHANDLER_HPP_

#include "EventHandler.hpp"
#include "ProcessState.hpp"
#include "Server.hpp"

/**
  This class is handling all ProcessStates.
  That means it is building them right and destryoing them when done.

  There is timeout functions going somewhere too...
  */
class ProcessHandler : public EventHandler {
    public:
        ProcessHandler(int socket, Server &server);
        ~ProcessHandler();

        void handle();

    private:
        void          transition_to_rbs(); ///< Transition to response building state;
        void          transition_to_rss(); ///< Transition to response sending state;
        ProcessState *_state;
};

#endif // INCLUDE_SRC_PROCESSHANDLER_HPP_
