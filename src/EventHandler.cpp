/* #####################################################################################################################

               """          EventHandler.cpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 14:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "EventHandler.hpp"

EventHandler::EventHandler(int socket, int port) : _socket(socket), _port(port) {}

EventHandler::~EventHandler() {}
