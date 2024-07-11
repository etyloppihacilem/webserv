#ifndef __INCLUDE_SRC_ROUTE_HPP_
#define __INCLUDE_SRC_ROUTE_HPP_

#include <string>
#include <vector>

#include "HttpMethods.hpp"
#include "ServerConfFields.hpp"

class Server;

class Route {
    public:
        Route();
        Route(Server&);
        Route(const std::string&, const std::string&, Server&);
        ~Route();

        std::vector<HttpMethod>     getMethods() const;
        std::string                 getRootDir() const;
        std::vector<std::string>    getIndexPage() const;
        bool                        getAutoindex() const;
        std::string                 getRedirCode() const;
        std::string                 getRedirPage() const;
        std::string                 getUploadPath() const;
        std::string                 getCgiPath() const;
        std::string                 getCgiExtension() const;
        std::string                 getLocation() const;

        bool                        hasMethods() const;
        bool                        hasRoot() const;
        bool                        hasIndexPage() const;
        bool                        hasAutoindex() const;
        bool                        hasRedir() const;
        bool                        hasUpload() const;
        bool                        hasCgiPath() const;
        bool                        hasCgiExtension() const;

        void                        setMethods(const ValueList&);
        void                        setRootDir(const ValueList&);
        void                        setIndexPage(const ValueList&);
        void                        setAutoindex(const ValueList&);
        void                        setRedirection(const ValueList&);
        void                        setUpload(const ValueList&);
        void                        setCgi(const ValueList&, const ValueList&);
        void                        setCgiPath(const ValueList&);
        void                        setCgiExtension(const ValueList&);

    private:
        std::string                 _location;
        std::vector<HttpMethod>     _methods;
        std::string                 _rootDir;
        std::vector<std::string>    _indexPage;
        bool                        _autoindex;
        std::string                 _uploadPath;
        std::string                 _redirCode;
        std::string                 _redirPage;
        std::string                 _cgiPath;
        std::string                 _cgiExtension;

        bool                        _indexPageSet;
        bool                        _autoindexSet;
        bool                        _rootDirSet;
        bool                        _methodsSet;
        bool                        _redirectionSet;
        bool                        _uploadSet;
        bool                        _cgiPathSet;
        bool                        _cgiExtensionSet;
};

#endif // !__INCLUDE_SRC_ROUTE__
