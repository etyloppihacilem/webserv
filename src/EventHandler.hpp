/* #####################################################################################################################

               """          EventHandler.hpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 14:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_SRC_EVENTHANDLER_HPP_
#define INCLUDE_SRC_EVENTHANDLER_HPP_

/**
  Base class to handlers (ProcessHandler and AcceptHandler)
  */
class EventHandler {
    public:
        EventHandler(int socket);
        virtual ~EventHandler() = 0;

        virtual void handle() = 0; ///< Run this in loop

    protected:
        int    _socket;
        // time_t _last_activity;
};

#endif // INCLUDE_SRC_EVENTHANDLER_HPP_
