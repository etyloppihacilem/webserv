#ifndef INCLUDE_SRC_SERVERMANAGER_HPP_
#define INCLUDE_SRC_SERVERMANAGER_HPP_

#include "EventHandler.hpp"
#include "Server.hpp"
#include "ServerReactor.hpp"
#include <exception>
#include <string>
#include <vector>

class ServerManager {
    public:
        static ServerManager *getInstance(const std::string &configFile = "");
        static void           deleteInstance();
        ~ServerManager();

        int  addClient(int socket_fd, int port, std::string client_IP);
        int  addCGIToddler(EventHandler *handler_miso, EventHandler *handler_mosi);
        void deleteClient(int socket_fd, EventHandler &handler);
        void listenToClient(int socket_fd, EventHandler &handler);
        void talkToClient(int socket_fd, EventHandler &handler);
        void run(void);

        const Server &getServer(const std::string &serverName, int port) const;
        const Server &getServer(int port) const;

        class ServerNotFoundWarn : public std::exception {
            public:
                ServerNotFoundWarn(std::string message = "") throw() : _message(message) {}

                virtual ~ServerNotFoundWarn() throw() {}

                const char *what() const throw() { return _message.c_str(); }

            private:
                std::string _message;
        };

        class FailToInitServerError : public std::exception {
            public:
                FailToInitServerError() throw() {}

                virtual ~FailToInitServerError() throw() {}
        };

        const ServerReactor &getReactor() const;

    private:
        ServerManager(const std::string &configFile);

        ServerManager(ServerManager &other) { (void) other; }

        void operator=(const ServerManager &other) { (void) other; }

        std::vector< Server > parseConfFile(const std::string &configFile);

        static ServerManager *_instance;
        std::vector< Server > _servers;
        ServerReactor         _reactor;
};

#endif // INCLUDE_SRC_SERVERMANAGER_HPP_
