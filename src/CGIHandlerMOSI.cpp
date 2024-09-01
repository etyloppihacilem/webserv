/* #####################################################################################################################

               """          CGIHandlerMOSI.cpp
        -\-    _|__
         |\___/  . \        Created on 01 Sep. 2024 at 21:23
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "CGIHandlerMOSI.hpp"
#include "CGIStrategy.hpp"
#include "EventHandler.hpp"
#include "ServerManager.hpp"
#include <ctime>
#include <unistd.h>

CGIHandlerMOSI::CGIHandlerMOSI(int MOSI_fd, CGIStrategy &strategy) : EventHandler(MOSI_fd), _strategy(strategy) {}

CGIHandlerMOSI::~CGIHandlerMOSI() {
    if (_socket_fd >= 0)
        close(_socket_fd);
}

time_t CGIHandlerMOSI::getTimeout() const {
    return CGI_TIMEOUT;
}

void CGIHandlerMOSI::handle() {
    if (_strategy.feed_CGI()) {
        // FIX: suicide delete client on self;
    }
} // Run this in loop

void CGIHandlerMOSI::timeout() {
    ServerManager::getInstance()->deleteClient(_socket_fd, *this);
}
