#include "AcceptHandler.hpp"
#include "EventHandler.hpp"
#include "Logger.hpp"
#include "ServerManager.hpp"
#include <cerrno>
#include <cstring>
#include <ctime>
#include <limits>
#include <netinet/in.h>
#include <ostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

AcceptHandler::AcceptHandler(int socket, int port) : EventHandler(socket, port) {}

AcceptHandler::~AcceptHandler() {}

time_t AcceptHandler::getTimeout() const {
    return std::numeric_limits< time_t >::max();
}

void AcceptHandler::handle() {
    int                client_fd;
    struct sockaddr_in socket_addr;
    socklen_t          socket_size = sizeof socket_addr;

    if ((client_fd = accept(_socket_fd, (struct sockaddr *) (&socket_addr), &socket_size)) == -1) {
        warn.log() << "accept: " << std::string(std::strerror(errno)) << std::endl;
        return;
    }
    ServerManager::getInstance()->addClient(client_fd, _port);
}

void AcceptHandler::timeout() {
    warn.log() << "Client timeout" << std::endl;
    ServerManager::getInstance()->deleteClient(_socket_fd);
}
