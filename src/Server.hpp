#ifndef INCLUDE_SRC_SERVER_HPP_
#define INCLUDE_SRC_SERVER_HPP_

#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Route.hpp"
#include "ServerConfFields.hpp"
#include "StringTokenizer.hpp"
#include <exception>
#include <map>
#include <set>
#include <string>
#include <vector>

#ifdef TESTING
# include "gtest/gtest.h"
#endif

class Server {
    public:
        typedef void (Server::*Setter)(const ValueList &);
        static Setter     fieldSetterList[COUNT_CONF_FIELD];
        static HttpMethod initMethod[1];

        Server();
        Server(StringTokenizer &);
        ~Server();

        std::vector< std::string >        getServerName() const;
        int                               getPort() const;
        std::string                       getRootDir() const;
        std::vector< std::string >        getIndexPage() const;
        bool                              getAutoindex() const;
        std::set< HttpMethod >            getMethods() const;
        int                               getMaxBodySize() const;
        std::map< HttpCode, std::string > getErrorPages() const;
        const Route                      &getRoute(const std::string &path) const;

        bool hasServeName(const std::string &serverName) const;
        bool hasRoute(const std::string &path) const;

        bool hasServeNameSet() const;
        bool hasListenSet() const;
        bool hasRootSet() const;
        bool hasIndexPageSet() const;
        bool hasAutoindexSet() const;
        bool hasMethodsSet() const;
        bool hasMaxBodySizeSet() const;

        class RouteNotFoundWarn : public std::exception {
            public:
                RouteNotFoundWarn(std::string message = "") throw() : _message(message) {}

                virtual ~RouteNotFoundWarn() throw() {}

                const char *what() const throw() { return _message.c_str(); }

            private:
                std::string _message;
        };

    private:
        std::vector< std::string >        _serverName;
        int                               _port;
        std::string                       _rootDir;
        std::vector< std::string >        _indexPage;
        bool                              _autoindex;
        std::set< HttpMethod >            _methods;
        int                               _maxBodySize;
        std::map< HttpCode, std::string > _errorPages;
        std::map< std::string, Route >    _routes;
        std::vector< bool >               _isFieldSet;

        void setServerName(const ValueList &);
        void setPort(const ValueList &);
        void setRootDir(const ValueList &);
        void setIndexPage(const ValueList &);
        void setAutoindex(const ValueList &);
        void setMethods(const ValueList &);
        void setMaxBodySize(const ValueList &);
        void addErrorPage(const ValueList &);
        void addRoute(Field &);

#ifdef TESTING
        friend class ServerRouteTestSuite;
        FRIEND_TEST(ServerRouteTestSuite, getRoute);
#endif // DEBUG
};

#endif // INCLUDE_SRC_SERVER_HPP_
