#ifndef INCLUDE_SRC_SERVER_HPP_
#define INCLUDE_SRC_SERVER_HPP_

#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Route.hpp"
#include "ServerConfFields.hpp"
#include "ServerGetRoute.hpp"
#include "StringTokenizer.hpp"
#include <map>
#include <set>
#include <string>
#include <vector>

#ifdef TESTING
# include "gtest/gtest.h"
#endif

class Server : public ServerGetRoute<> {
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

        bool hasServeName(const std::string &serverName) const;

        bool hasServeNameSet() const;
        bool hasListenSet() const;
        bool hasRootSet() const;
        bool hasIndexPageSet() const;
        bool hasAutoindexSet() const;
        bool hasMethodsSet() const;
        bool hasMaxBodySizeSet() const;

    private:
        std::vector< std::string >        _serverName;
        int                               _port;
        std::string                       _rootDir;
        std::vector< std::string >        _indexPage;
        bool                              _autoindex;
        std::set< HttpMethod >            _methods;
        int                               _maxBodySize;
        std::map< HttpCode, std::string > _errorPages;
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
