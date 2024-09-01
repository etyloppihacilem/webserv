/* #####################################################################################################################

               """          CGIHandlerMISO.cpp
        -\-    _|__
         |\___/  . \        Created on 01 Sep. 2024 at 21:23
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "CGIHandlerMISO.hpp"
#include "CGIStrategy.hpp"
#include "CGIWriter.hpp"
#include "EventHandler.hpp"
#include "ServerManager.hpp"
#include <ctime>
#include <unistd.h>

CGIHandlerMISO::CGIHandlerMISO(int MISO_fd, CGIStrategy &strategy, CGIWriter &writer) :
    EventHandler(MISO_fd),
    _strategy(strategy),
    _writer(writer) {}

CGIHandlerMISO::~CGIHandlerMISO() {
    if (_socket_fd >= 0)
        close(_socket_fd);
}

time_t CGIHandlerMISO::getTimeout() const {
    return CGI_TIMEOUT;
}

void CGIHandlerMISO::handle() {
    if (_writer.read_from_child())
        timeout(); // TS-14
} // Run this in loop

void CGIHandlerMISO::timeout() {
    ServerManager::getInstance()->deleteClient(_socket_fd, *this);
}
