#ifndef INCLUDE_SRC_SERVER_HPP_
#define INCLUDE_SRC_SERVER_HPP_

#include <string>
#include <map>

#include "ServerConfFields.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Route.hpp"

class Server{
    private:
        std::vector<std::string>        _serverName;
        unsigned int                    _port;
        bool                            _autoindex;
        std::vector<HttpMethod>         _methods;
        std::string                     _rootDir;
        std::vector<std::string>        _indexPage;
        int                             _maxBodySize;
        std::map<std::string, Route>    _routes;
        std::map<HttpCode, std::string> _errorPages;

    public:
        Server();
        Server(const std::string&);
        ~Server();

        bool                            hasServeName(const std::string &serverName) const;
        std::vector<std::string>        getServerName() const;

        unsigned int                    getPort() const;

        bool                            getAutoindex() const;
        std::vector<HttpMethod>         getMethods() const;
        std::string                     getRootDir() const;
        std::vector<std::string>        getIndexPage() const;
        std::map<HttpCode, std::string> getErrorPages() const;
        bool                            hasRoute(const std::string &path) const;
        Route                           &getRoute(const std::string &path);

        void                            setServerName(const ValueList &);
        void                            setPort(const ValueList &);
        void                            setAutoindex(const ValueList &);
        void                            setMethods(const ValueList &);
        void                            setRootDir(const ValueList &);
        void                            setIndexPage(const ValueList &);
        void                            setMaxBodySize(const ValueList &);
        void                            addRoute(const Field &);
        void                            addErrorPage(const ValueList &);

        // Route *getRoute(const std::string &path);
};

#endif // INCLUDE_SRC_SERVER_HPP_
