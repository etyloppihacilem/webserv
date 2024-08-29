#include "ServerReactor.hpp"
#include "AcceptHandler.hpp"
#include "EventHandler.hpp"
#include "Logger.hpp"
#include "ProcessHandler.hpp"
#include "Server.hpp"
#include <asm-generic/socket.h>
#include <cerrno>
#include <csignal>
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

extern sig_atomic_t g_signal;

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
            warn.log() << "ServerReactor: initNetwork: " << it->getPort() << " is already in use." << std::endl;
            continue;
        }

        int                server_fd = initServerSocket(it->getPort());
        struct epoll_event event;
        event.events   = EPOLLIN | EPOLLERR | EPOLLHUP;
        event.data.ptr = new AcceptHandler(server_fd, it->getPort());
        errno          = 0;
        if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
            delete static_cast< EventHandler * >(event.data.ptr);
            close(event.data.fd);
            throw std::runtime_error("ServerReactor: epoll_ctl_add: " + std::string(std::strerror(errno)));
        }

        listeningPort.insert(it->getPort());
        _eventHandlers.insert(static_cast< EventHandler * >(event.data.ptr));
         info.log() << "ServerReactor: port " << it->getPort() << " is open for client connection." << std::endl;
    }
}

ServerReactor::ServerReactor(const std::vector< Server > &servers) : _epoll_fd(-1) {
    if ((_epoll_fd = epoll_create(EPOLL_INIT_CONNECTION)) == -1) {
        error.log();
        throw std::runtime_error("ServerReactor: epoll_create: " + std::string(std::strerror(errno)));
    }
    initNetwork(servers);
}

ServerReactor::~ServerReactor(void) {
    close(_epoll_fd);

    for (std::set< EventHandler * >::iterator it = _eventHandlers.begin(); it != _eventHandlers.end(); ++it)
        delete *it;
}

int ServerReactor::addClient(int client_fd, int port, std::string client_IP) {
    info.log() << "ServerReactor: New connection on client socket " << client_fd << " from " << client_IP << std::endl;
    if (_eventHandlers.size() >= MAX_TOTAL_CONNECTION) {
        warn.log() << "ServerReactor: addClient: max connection reached." << std::endl;
        return -1;
    }

    struct epoll_event event;
    event.events   = EPOLLIN | EPOLLERR | EPOLLHUP;
    event.data.ptr = new ProcessHandler(client_fd, port, client_IP);
    errno          = 0;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
        delete static_cast< EventHandler * >(event.data.ptr);
        close(event.data.fd);
        throw std::runtime_error("ServerReactor: epoll_ctl_add: " + std::string(std::strerror(errno)));
    }

    _eventHandlers.insert(static_cast< EventHandler * >(event.data.ptr));
    return 0;
}

// NOTE: for info should we pass the client IP?
void ServerReactor::deleteClient(int socket_fd) {
    info.log() << "ServerReactor: Delete client socket " << socket_fd << std::endl;
    errno = 0;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, socket_fd, 0) == -1)
        throw std::runtime_error("ServerReactor: epoll_ctl_del: " + std::string(std::strerror(errno)));
}

void ServerReactor::listenToClient(int socket_fd, EventHandler &handler) {
    struct epoll_event event;
    event.events   = EPOLLIN | EPOLLERR | EPOLLHUP;
    event.data.fd  = socket_fd;
    event.data.ptr = &handler;
    errno          = 0;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, event.data.fd, &event) == -1)
        throw std::runtime_error("ServerReactor: epoll_ctl_mod: " + std::string(std::strerror(errno)));
}

void ServerReactor::talkToClient(int socket_fd, EventHandler &handler) {
    struct epoll_event event;
    event.events   = EPOLLOUT | EPOLLERR | EPOLLHUP;
    event.data.fd  = socket_fd;
    event.data.ptr = &handler;
    errno          = 0;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, event.data.fd, &event) == -1)
        throw std::runtime_error("ServerReactor: epoll_ctl_mod: " + std::string(std::strerror(errno)));
}

void ServerReactor::run() {

    struct epoll_event events[MAX_TOTAL_CONNECTION];
    int                event_count;

    info.log() << "ServerReactor: JEON is listening..." << std::endl;
    while (g_signal == false) {
        errno = 0;
        if ((event_count = epoll_wait(_epoll_fd, events, MAX_TOTAL_CONNECTION, 16000)) == -1) {
            if (errno == EINTR)
                return;
            error.log() << "ServerReactor: epoll_wait: " << std::string(std::strerror(errno)) << std::endl;
        }

        for (int i = 0; i < event_count; ++i) {
            if (_eventHandlers.find(static_cast< EventHandler * >(events[i].data.ptr)) == _eventHandlers.end())
                continue;
            info.log() << "ServerReactor: event on socket "
                       << static_cast< EventHandler * >(events[i].data.ptr)->getSocketFd() << std::endl;
            static_cast< EventHandler * >(events[i].data.ptr)->handle();
        }
        if (event_count == 0)
            for (std::set< EventHandler * >::iterator it = _eventHandlers.begin(); it != _eventHandlers.end(); ++it)
                (*it)->checkTimeout();
    }
}

#ifdef TESTING
std::set< EventHandler * > &ServerReactor::getEventHandler() {
    return _eventHandlers;
}
#endif
