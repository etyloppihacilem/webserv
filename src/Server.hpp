#ifndef INCLUDE_SRC_SERVER_HPP_
#define INCLUDE_SRC_SERVER_HPP_

#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Route.hpp"
#include "ServerConfFields.hpp"
#include <exception>
#include <string>
#include <map>
#include <vector>

class Server{
    class RouteNotFoundWarn: public std::exception{
        public:
            RouteNotFoundWarn(std::string message = "") throw ():
                _message(message) {}

            virtual ~RouteNotFoundWarn() throw () {}

            const char  *what() const throw () {
                return _message.c_str();
            }

        private:
            std::string _message;
    };

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

        void                            setServerName(const ValueList&);
        void                            setPort(const ValueList&);
        void                            setRootDir(const ValueList&);
        void                            setIndexPage(const ValueList&);
        void                            setAutoindex(const ValueList&);
        void                            setMethods(const ValueList&);
        void                            setMaxBodySize(const ValueList&);
        void                            addRoute(const Field&);
        void                            addErrorPage(const ValueList&);

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

        bool                            _serverNameSet;
        bool                            _portSet;
        bool                            _autoindexSet;
        bool                            _indexPageSet;
        bool                            _methodsSet;
        bool                            _rootDirSet;
        bool                            _maxBodySizeSet;
};

#endif // INCLUDE_SRC_SERVER_HPP_
