#ifndef INCLUDE_SRC_ROUTE_HPP_
#define INCLUDE_SRC_ROUTE_HPP_

#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "ServerConfFields.hpp"
#include "StringTokenizer.hpp"
#include <cstdlib>
#include <set>
#include <string>
#include <vector>

class Server;

class Route {
    public:
        typedef void (Route::*Setter)(const ValueList &);
        static Setter fieldSetterList[COUNT_CONF_FIELD];

        Route();
        Route(Server &);
        Route(const std::string &, StringTokenizer &, Server &);
        ~Route();

        const std::string                &getLocation() const;
        const std::string                &getRootDir() const;
        const std::vector< std::string > &getIndexPage() const;
        bool                              getAutoindex() const;
        const std::set< HttpMethod >     &getMethods() const;
        std::size_t                       getMaxBodySize() const;
        const std::string                &getUploadPath() const;
        HttpCode                          getRedirCode() const;
        const std::string                &getRedirPage() const;
        const std::string                &getCgiPath() const;
        const std::string                &getCgiExtension() const;

        bool hasRootSet() const;
        bool hasIndexPageSet() const;
        bool hasAutoindexSet() const;
        bool hasMethodsSet() const;
        bool hasMaxBodySizeSet() const;
        bool hasUploadSet() const;
        bool hasRedirSet() const;
        bool hasCgiPathSet() const;
        bool hasCgiExtensionSet() const;

    private:
        std::string                _location;
        std::string                _rootDir;
        std::vector< std::string > _indexPage;
        bool                       _autoindex;
        std::set< HttpMethod >     _methods;
        std::size_t                _maxBodySize;
        std::string                _uploadPath;
        HttpCode                   _redirCode;
        std::string                _redirPage;
        std::string                _cgiPath;
        std::string                _cgiExtension;
        std::vector< bool >        _isFieldSet;

        void setRootDir(const ValueList &);
        void setIndexPage(const ValueList &);
        void setAutoindex(const ValueList &);
        void setMethods(const ValueList &);
        void setMaxBodySize(const ValueList &);
        void setUpload(const ValueList &);
        void setRedirection(const ValueList &);
        void setCgiPath(const ValueList &);
        void setCgiExtension(const ValueList &);
};

#endif // INCLUDE_SRC_ROUTE_HPP
