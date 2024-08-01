/* #####################################################################################################################

               """          ProcessHandler.cpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 14:33
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ProcessHandler.hpp"
#include "EventHandler.hpp"

ProcessHandler::ProcessHandler(int fd) : EventHandler(fd) {}

ProcessHandler::~ProcessHandler() {}
