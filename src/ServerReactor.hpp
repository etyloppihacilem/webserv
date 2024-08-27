#ifndef INCLUDE_SRC_SERVERREACTOR_HPP_
#define INCLUDE_SRC_SERVERREACTOR_HPP_

#include "EventHandler.hpp"
#include "Server.hpp"
#include <set>
#include <vector>

class ServerReactor {
    public:
        ServerReactor();
        ServerReactor(const std::vector< Server > &servers);
        ~ServerReactor();

        int  initServerSocket(int port);
        void initNetwork(const std::vector< Server > &servers);

        int  addClient(int socket_fd, int port);
        void ignoreClient(int socket_fd);
        void deleteClient(int socket_fd);
        void listenToClient(int socket_fd, EventHandler &handler);
        void talkToClient(int socket_fd, EventHandler &handler);

        void run();

    private:
        int                        _epoll_fd;
        std::set< EventHandler * > _eventHandlers;
};

#endif // !INCLUDE_SRC_SERVERREACTOR_HPP_
