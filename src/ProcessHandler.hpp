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

/**
  This class is handling all ProcessStates.
  */
class ProcessHandler : public EventHandler {
    public:
        ProcessHandler(int fd);
        ~ProcessHandler();

    private:
};

#endif // INCLUDE_SRC_PROCESSHANDLER_HPP_
