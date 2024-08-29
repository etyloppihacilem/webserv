#ifndef INCLUDE_SRC_SERVERREACTOR_HPP_
#define INCLUDE_SRC_SERVERREACTOR_HPP_

#include "EventHandler.hpp"
#include "Server.hpp"
#include <set>
#include <string>
#include <vector>

#define EPOLL_INIT_CONNECTION 100
#define MAX_SERVER_BACKLOG 100
#define MAX_CLIENT_CONNECTION 5
#define MAX_TOTAL_CONNECTION 512 // or 1000 but risky to access file rssources.

class ServerReactor {
    public:
        ServerReactor();
        ServerReactor(const std::vector< Server > &servers);
        ~ServerReactor();

        int  initServerSocket(int port);
        void initNetwork(const std::vector< Server > &servers);

        int  addClient(int socket_fd, int port, std::string client_IP);
        void deleteClient(int socket_fd);
        void listenToClient(int socket_fd, EventHandler &handler);
        void talkToClient(int socket_fd, EventHandler &handler);

        void run();

    private:
        int                        _epoll_fd;
        std::set< EventHandler * > _eventHandlers;
};

#endif // !INCLUDE_SRC_SERVERREACTOR_HPP_
