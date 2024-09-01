/* #####################################################################################################################

               """          EventHandler.cpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 14:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "EventHandler.hpp"
#include <cerrno>
#include <cstring>
#include <ctime>

EventHandler::EventHandler(int socket_fd, int port) : _socket_fd(socket_fd), _port(port), _last_activity(time(NULL)) {}

EventHandler::~EventHandler() {}

int EventHandler::getSocketFd(void) const {
    return _socket_fd;
}

int EventHandler::getPort(void) const {
    return _port;
}

bool EventHandler::checkTimeout(void) {
    if ((time(NULL) - _last_activity) > getTimeout())
        return true;
    return false;
}

void EventHandler::updateLastsActivity(void) {
    _last_activity = time(NULL);
}
