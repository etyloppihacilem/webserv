#include "AcceptHandler.hpp"
#include "EventHandler.hpp"
#include "Logger.hpp"
#include "ServerManager.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <limits>
#include <netinet/in.h>
#include <ostream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

AcceptHandler::AcceptHandler(int socket, int port) : EventHandler(socket, port) {
    debug.log() << "AcceptHandler: created on socket " << _socket_fd << " for port " << _port << "." << std::endl;
}

AcceptHandler::~AcceptHandler() {
    debug.log() << "AcceptHandler: deleted on socket " << _socket_fd << " for port " << _port << "." << std::endl;
    close(_socket_fd);
}

time_t AcceptHandler::getTimeout() const {
    return std::numeric_limits< time_t >::max();
}

void AcceptHandler::handle() {
    int                client_fd;
    struct sockaddr_in socket_addr;
    socklen_t          socket_size = sizeof socket_addr;

    info.log() << "AcceptHandler: port " << _port << " received a new event!" << std::endl;
    if ((client_fd = accept(_socket_fd, (struct sockaddr *) (&socket_addr), &socket_size)) == -1) {
        warn.log() << "AcceptHandler: accept: " << std::string(std::strerror(errno)) << std::endl;
        return;
    }

    errno = 0;
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
        close(client_fd);
        throw std::runtime_error("ServerReactor: fcntl: " + std::string(std::strerror(errno)));
    }
    char s[INET_ADDRSTRLEN];
    inet_ntop(socket_addr.sin_family, (struct sockaddr *) &socket_addr, s, sizeof s);
    if (ServerManager::getInstance()->addClient(client_fd, _port, std::string(s)) == -1) {
        warn.log() << "AcceptHandler: unable to add more client to epoll list." << std::endl;
        close(client_fd);
    }
}

void AcceptHandler::timeout() {
    updateLastsActivity();
}

void AcceptHandler::save_mem() {
    debug.log() << "(i) AcceptHandler on " << _port << " saved mem !" << std::endl;
}
