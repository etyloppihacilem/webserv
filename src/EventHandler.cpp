/* #####################################################################################################################

               """          EventHandler.cpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 14:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "EventHandler.hpp"
#include "Server.hpp"

EventHandler::EventHandler(int socket, Server &server) : _socket(socket), _server(server) {}

EventHandler::~EventHandler() {}
