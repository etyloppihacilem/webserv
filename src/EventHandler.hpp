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
#include <ctime>

// nginx default values
#define CONNECTION_TIMEOUT 180 // second
#define REQUEST_TIMEOUT 60     // second
#define INPUT_TIMEOUT 60       // second
#define OUTPUT_TIMEOUT 60     // second
#define CGI_TIMEOUT 60        // second

class EventHandler {
    public:
        EventHandler(int socket_fd, int port);
        virtual ~EventHandler(void) = 0;

        int getSocketFd(void) const;
        virtual time_t getTimeout(void) const = 0;

        virtual void handle(void) = 0; // Run this in loop
        virtual void timeout(void) = 0;
        void checkTimeout(void);
        void updateLastsActivity(void);

    protected:
        int _socket_fd;
        int _port;
        time_t _last_activity;
};

#endif // INCLUDE_SRC_EVENTHANDLER_HPP_
