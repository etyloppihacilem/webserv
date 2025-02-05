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
#include "HttpStatusCodes.hpp"
#include "ProcessState.hpp"
#include <ctime>
#include <string>

/**
  This class is handling all ProcessStates.
  That means it is building them right and destryoing them when done.

  There is timeout functions going somewhere too...
  */
class ProcessHandler : public EventHandler {
    public:
        ProcessHandler(int socket_fd, int port, std::string client_IP);
        ~ProcessHandler();

        time_t getTimeout() const;

        void handle();
        void timeout();

        void save_mem();

    private:
        void clean_state();
        void transition_to_rbs();          ///< Transition to response building state;
        void transition_to_rss();          ///< Transition to response sending state;
        bool rewind_to_rbs(HttpCode code); ///< To reply an error.

        ProcessState *_state;
        std::string   _client_IP;
        size_t        _rewind_count;
};

#endif // INCLUDE_SRC_PROCESSHANDLER_HPP_
