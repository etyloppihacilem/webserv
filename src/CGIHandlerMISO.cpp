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

CGIHandlerMISO::CGIHandlerMISO(
    int                MISO_fd,
    CGIStrategy       &strategy,
    CGIWriter         &writer,
    const std::string &temp_file,
    int                process_fd
) :
    EventHandler(MISO_fd),
    _strategy(strategy),
    _writer(writer),
    _temp_file(temp_file),
    _process_fd(process_fd) {
    debug.log() << "CGIHandlerMISO: created on pipe " << _socket_fd << " for child " << _strategy.get_child_pid() << "."
                << std::endl;
}

CGIHandlerMISO::~CGIHandlerMISO() {
    debug.log() << "CGIHandlerMISO: closed pipe " << _socket_fd << " for child " << _strategy.get_child_pid() << "."
                << std::endl;
    debug.log() << "Removing " << _temp_file << std::endl;
    remove(_temp_file.c_str());
    if (_socket_fd >= 0)
        close(_socket_fd);
    _strategy.removeMISO();
}

time_t CGIHandlerMISO::getTimeout() const {
    return CGI_TIMEOUT;
}

void CGIHandlerMISO::handle() {
    info.log() << "CGIHandlerMISO: Child " << _strategy.get_child_pid() << " on pipe " << _socket_fd
               << " received a new event!" << std::endl;
    if (_writer.read_from_child()) {
        _writer.init();
        _strategy.is_done_building();
        ServerManager::getInstance()->getReactor().getCGIHandler(_process_fd)->handle();
        ServerManager::getInstance()->deleteClient(getSocketFd(), *this);
        return;
    }
    if (!_writer.init()) {
        _strategy.is_done_building();
        ServerManager::getInstance()->talkToClient(
            _process_fd, *ServerManager::getInstance()->getReactor().getCGIHandler(_process_fd)
        );
    }
} // Run this in loop

void CGIHandlerMISO::timeout() {
    warn.log() << "CGIHandlerMISO: Child " << _strategy.get_child_pid() << " on pipe " << _socket_fd << " timed out! ("
               << getTimeout() << "s)" << std::endl;
    ServerManager::getInstance()->deleteClient(_socket_fd, *this);
}
