#include "ServerReactor.hpp"
#include "AcceptHandler.hpp"
#include "EventHandler.hpp"
#include "Logger.hpp"
#include "ProcessHandler.hpp"
#include "Server.hpp"
#include "ServerConfFields.hpp"
#include <asm-generic/socket.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

ServerReactor::ServerReactor(void) : _epoll_fd(-1) {}

int ServerReactor::initServerSocket(int port) {
    struct sockaddr_in address;
    int                server_fd = -1;
    errno                        = 0;
    if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        throw std::runtime_error("ServerReactor: socket: " + std::string(std::strerror(errno)));

    errno = 0;
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1) {
        close(server_fd);
        throw std::runtime_error("ServerReactor: fcntl: " + std::string(std::strerror(errno)));
    }

    int sockopt = 1;
    errno       = 0;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1) {
        close(server_fd);
        throw std::runtime_error("ServerReactor: setsockopt: " + std::string(std::strerror(errno)));
    }

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(port);
    errno                   = 0;
    if (bind(server_fd, (struct sockaddr *) &address, sizeof address) == -1) {
        close(server_fd);
        throw std::runtime_error("ServerReactor: bind: " + std::string(std::strerror(errno)));
    }

    errno = 0;
    if (listen(server_fd, MAX_SERVER_BACKLOG) == -1) {
        close(server_fd);
        throw std::runtime_error("ServerReactor: listen: " + std::string(std::strerror(errno)));
    }

    return server_fd;
}

void ServerReactor::initNetwork(const std::vector< Server > &servers) {
    std::set< int > listeningPort;
    for (std::vector< Server >::const_iterator it = servers.begin(); it != servers.end(); ++it) {
        if (listeningPort.find(it->getPort()) != listeningPort.end()) {
            warn.log() << "ServerReactor: initNetwork: " << it->getPort() << " is already use." << std::endl;
            continue;
        }

        struct epoll_event event;
        event.events   = EPOLLIN | EPOLLERR | EPOLLHUP;
        event.data.fd  = initServerSocket(it->getPort());
        event.data.ptr = new AcceptHandler(event.data.fd, it->getPort());
        errno          = 0;
        if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event) == -1) {
            delete static_cast< EventHandler * >(event.data.ptr);
            close(event.data.fd);
            throw std::runtime_error("ServerReactor: epoll_ctl: " + std::string(std::strerror(errno)));
        }

        listeningPort.insert(it->getPort());
        this->_eventHandlers.insert(static_cast< EventHandler * >(event.data.ptr));
    }
}

ServerReactor::ServerReactor(const std::vector< Server > &servers) : _epoll_fd(-1) {
    if ((this->_epoll_fd = epoll_create(EPOLL_INIT_CONNECTION)) == -1) {
        error.log();
        throw std::runtime_error("ServerReactor: epoll_create: " + std::string(std::strerror(errno)));
    }
    (void) servers;
}

ServerReactor::~ServerReactor(void) {
    close(this->_epoll_fd);

    for (std::set< EventHandler * >::iterator it = this->_eventHandlers.begin(); it != this->_eventHandlers.end(); ++it)
        delete *it;
}

int ServerReactor::addClient(int socket_fd, int port) {

    if (this->_eventHandlers.size() >= MAX_TOTAL_CONNECTION) {
        warn.log() << "ServerReactor: addClient: max connection reached." << std::endl;
        return -1;
    }

    struct epoll_event event;
    event.events   = EPOLLIN | EPOLLERR | EPOLLHUP;
    event.data.fd  = socket_fd;
    event.data.ptr = new ProcessHandler(event.data.fd, port);
    errno          = 0;
    if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event) == -1) {
        delete static_cast< EventHandler * >(event.data.ptr);
        close(event.data.fd);
        throw std::runtime_error("ServerReactor: epoll_ctl: " + std::string(std::strerror(errno)));
    }

    this->_eventHandlers.insert(static_cast< EventHandler * >(event.data.ptr));
    return 0;
}

void ServerReactor::ignoreClient(int socket_fd) {
    (void) socket_fd;
}

void ServerReactor::deleteClient(int socket_fd) {
    (void) socket_fd;
}

void ServerReactor::listenToClient(int socket_fd, EventHandler &handler) {
    (void) socket_fd;
    (void) handler;
}

void ServerReactor::talkToClient(int socket_fd, EventHandler &handler) {
    (void) socket_fd;
    (void) handler;
}
