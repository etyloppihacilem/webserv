/* #####################################################################################################################

               """          EventHandler.hpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 14:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_SRC_EVENTHANDLER_HPP_
#define INCLUDE_SRC_EVENTHANDLER_HPP_

// TODO: securiser tous les read et les send (write)...

/**
  Base class to handlers (ProcessHandler and AcceptHandler)
  */
class EventHandler {
    public:
        EventHandler(int fd);
        virtual ~EventHandler() = 0;

    protected:
        int _fd;
};

#endif  // INCLUDE_SRC_EVENTHANDLER_HPP_
