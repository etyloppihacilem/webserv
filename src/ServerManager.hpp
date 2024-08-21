#ifndef INCLUDE_SRC_SERVERMANAGER_HPP_
#define INCLUDE_SRC_SERVERMANAGER_HPP_

#include "Server.hpp"
#include <exception>
#include <string>
#include <vector>

class ServerManager {
    public:
        static ServerManager *getInstance(const std::string &configFile);
        void                  deleteInstance();
        ~ServerManager();

        Server &getServer(const std::string &serverName, int port);
        Server &getServer(int port);

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

    private:
        ServerManager(const std::string &configFile);

        ServerManager(ServerManager &other) { (void) other; }

        void operator=(const ServerManager &other) { (void) other; }

        static ServerManager *_instance;
        std::vector< Server > _servers;

        // ServerReactor _reactor;
};

#endif // INCLUDE_SRC_SERVERMANAGER_HPP_
