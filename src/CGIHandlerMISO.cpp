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
#include "Logger.hpp"
#include "ServerManager.hpp"
#include <cstdio>
#include <ctime>
#include <ostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

CGIHandlerMISO::CGIHandlerMISO(int MISO_fd, CGIStrategy &strategy, CGIWriter &writer, const std::string &temp_file) :
    EventHandler(MISO_fd),
    _strategy(strategy),
    _writer(writer),
    _temp_file(temp_file) {
    debug.log() << "CGIHandlerMISO: created on pipe " << _socket_fd << " for child " << _strategy.get_child_pid() << "."
                << std::endl;
}

CGIHandlerMISO::~CGIHandlerMISO() {
    debug.log() << "CGIHandlerMISO: closed pipe " << _socket_fd << " for child " << _strategy.get_child_pid() << "."
                << std::endl;
    remove(_temp_file.c_str());
    if (_socket_fd >= 0)
        close(_socket_fd);
    _strategy.removeMISO();
    // Stuff to end _strategy BUT not here
    // _strategy.is_done_building();
    // EventHandler *CGI_process = ServerManager::getInstance()->getReactor().getCGIHandler(_process_fd);
    // if (CGI_process) {
    //     CGI_process->handle();
    // }
}

time_t CGIHandlerMISO::getTimeout() const {
    return CGI_TIMEOUT;
}

void CGIHandlerMISO::handle() {
    info.log() << "CGIHandlerMISO: Child " << _strategy.get_child_pid() << " on pipe " << _socket_fd
               << " received a new event!" << std::endl;
    if (_writer.read_from_child()) {
        _writer.init();
        ServerManager::getInstance()->deleteClient(_socket_fd, *this);
        return;
    }
    _writer.init();
} // Run this in loop

void CGIHandlerMISO::timeout() {
    warn.log() << "CGIHandlerMISO: Child " << _strategy.get_child_pid() << " on pipe " << _socket_fd << " timed out! ("
               << getTimeout() << "s)" << std::endl;
    ServerManager::getInstance()->deleteClient(_socket_fd, *this);
}
