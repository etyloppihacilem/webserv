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
#include "Logger.hpp"
#include "ServerManager.hpp"
#include <ctime>
#include <ostream>
#include <unistd.h>

CGIHandlerMOSI::CGIHandlerMOSI(int MOSI_fd, CGIStrategy &strategy) : EventHandler(MOSI_fd), _strategy(strategy) {
    debug.log() << "CGIHandlerMOSI: created on pipe " << _socket_fd << " for child " << strategy.get_child_pid() << "."
                << std::endl;
}

CGIHandlerMOSI::~CGIHandlerMOSI() {
    if (_socket_fd >= 0)
        close(_socket_fd);
    debug.log() << "CGIHandlerMOSI: closed pipe " << _socket_fd << " for child " << _strategy.get_child_pid() << "."
                << std::endl;
}

time_t CGIHandlerMOSI::getTimeout() const {
    return CGI_TIMEOUT;
}

void CGIHandlerMOSI::handle() {
    info.log() << "CGIHandlerMOSI: Child " << _strategy.get_child_pid() << " on pipe " << _socket_fd
               << " received a new event!" << std::endl;
    if (_strategy.feed_CGI()) {
        // FIX: suicide delete client on self;
    }
} // Run this in loop

void CGIHandlerMOSI::timeout() {
    warn.log() << "CGIHandlerMOSI: Child " << _strategy.get_child_pid() << " on pipe " << _socket_fd << " timed out! ("
               << getTimeout() << "s)" << std::endl;
    ServerManager::getInstance()->deleteClient(_socket_fd, *this);
}
