#include "Server.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "Route.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfLogging.hpp"
#include "ServerConfSetter.hpp"
#include "ServerConfTokenize.hpp"
#include "StringTokenizer.hpp"
#include "StringUtils.hpp"
#include <algorithm>
#include <cstddef>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

Server::Setter Server::fieldSetterList[COUNT_CONF_FIELD] = { 0,
                                                             0,
                                                             0,
                                                             &Server::addErrorPage,
                                                             &Server::setServerName,
                                                             &Server::setPort,
                                                             &Server::setRootDir,
                                                             &Server::setIndexPage,
                                                             &Server::setAutoindex,
                                                             &Server::setMethods,
                                                             &Server::setMaxBodySize,
                                                             0,
                                                             0,
                                                             0,
                                                             0 };

HttpMethod Server::initMethod[1] = { GET };

Server::Server() :
    _serverName(1, "localhost"),
    _port(8080),
    _rootDir("www"),
    _indexPage(1, "index.html"),
    _autoindex(true),
    _methods(initMethod, initMethod + 1),
    _maxBodySize(1000000),
    _errorPages(),
    _routes(),
    _isFieldSet(COUNT_CONF_FIELD, false) {
    _routes["/"] = Route(*this);
}

Server::Server(StringTokenizer &tokenizedServer) :
    _serverName(1, "localhost"),
    _port(8080),
    _rootDir("www"),
    _indexPage(1, "index.html"),
    _autoindex(true),
    _methods(initMethod, initMethod + 1),
    _maxBodySize(1000000),
    _errorPages(),
    _routes(),
    _isFieldSet(COUNT_CONF_FIELD, false) {

    logParsingStarted(server, tokenizedServer.remainingString());

    std::vector< Field > locationList;
    while (tokenizedServer.hasMoreTokens()) {
        try {
            if (tokenizedServer.peakToken() == ConfFieldString(location)) {
                locationList.push_back(tokenizeLocation(tokenizedServer));
                continue;
            }
            Field     fieldContent = tokenizeField(tokenizedServer);
            ConfField fieldCode    = getFieldCode(fieldContent.first);
            ValueList values       = getFieldValues(fieldContent.second);
            if (isServerConfField(fieldCode) == wrongField) {
                warn.log() << fieldContent.first << ": is not a valide Server Conf Field" << std::endl;
                continue;
            }
            if (fieldCode != error_page && _isFieldSet[fieldCode]) {
                logFieldRedefinition(fieldCode, values);
                continue;
            }
            (this->*Server::fieldSetterList[fieldCode])(values);
            _isFieldSet[fieldCode] = true;
        } catch (ServerConfWarn &e) {
            continue;
        }
    }
    for (std::vector< Field >::iterator it = locationList.begin(); it < locationList.end(); ++it)
        this->addRoute(*it);
    if (!hasRoute("/"))
        _routes["/"] = Route(*this);

    logParsingEnded(server);
}

Server::~Server() {}

std::vector< std::string > Server::getServerName() const {
    return _serverName;
}

int Server::getPort() const {
    return _port;
}

std::string Server::getRootDir() const {
    return _rootDir;
}

std::vector< std::string > Server::getIndexPage() const {
    return _indexPage;
}

bool Server::getAutoindex() const {
    return _autoindex;
}

std::set< HttpMethod > Server::getMethods() const {
    return _methods;
}

int Server::getMaxBodySize() const {
    return _maxBodySize;
}

std::map< HttpCode, std::string > Server::getErrorPages() const {
    return _errorPages;
}

// TODO: get this in fakeRoute
const Route &Server::getRoute(const std::string &path) const {
    if (path[0] != '/') {
        warn.log() << "target '" << path << "' is not starting with '/', wrong target. Choosing default route '/'."
                   << std::endl;
        if (hasRoute("/"))
            return _routes.at("/");
        error.log() << "No default route with wrong target (not starting with /): '" << path << "'" << std::endl;
        throw RouteNotFoundWarn(path);
    }

    size_t      nextSlash = path.find('/');
    std::string lastFound = "/";
    std::string testing   = path.substr(0, nextSlash + 1);

    while (hasRoute(testing)) {
        lastFound = testing;
        if (testing.size() == path.size())
            break ;
        nextSlash = path.find('/', nextSlash + 1);
        if (nextSlash == std::string::npos) {
            testing = path;
            continue;
        }
        testing = path.substr(0, nextSlash + 1);
    }

    /*StringTokenizer tokenizedPath(path, '|');*/
    return _routes.at(lastFound); // will return "/" route if default
}

bool Server::hasServeName(const std::string &serverName) const {
    std::vector< std::string >::const_iterator name = _serverName.end();

    name = std::find(_serverName.begin(), _serverName.end(), serverName);
    if (name == _serverName.end())
        return false;
    else
        return true;
}

inline bool Server::hasRoute(const std::string &path) const {
    return _routes.find(path) == _routes.end() ? false : true;
}

bool Server::hasServeNameSet() const {
    return _isFieldSet[server_name];
}

bool Server::hasListenSet() const {
    return _isFieldSet[listen];
}

bool Server::hasRootSet() const {
    return _isFieldSet[root];
}

bool Server::hasIndexPageSet() const {
    return _isFieldSet[index_f];
}

bool Server::hasAutoindexSet() const {
    return _isFieldSet[autoindex];
}

bool Server::hasMethodsSet() const {
    return _isFieldSet[methods];
}

bool Server::hasMaxBodySizeSet() const {
    return _isFieldSet[client_max_body_size];
}

void Server::setServerName(const ValueList &values) {
    _serverName = setFieldServerName(values);
}

void Server::setPort(const ValueList &values) {
    _port = setFieldListen(values);
}

void Server::setRootDir(const ValueList &values) {
    _rootDir = clean_trailing_slash(setFieldRoot(values));
}

void Server::setIndexPage(const ValueList &values) {
    _indexPage = setFieldIndex(values);
}

void Server::setAutoindex(const ValueList &values) {
    _autoindex = setFieldAutoindex(values);
}

void Server::setMethods(const ValueList &values) {
    _methods = setFieldMethods(values);
}

void Server::setMaxBodySize(const ValueList &values) {
    _maxBodySize = setFieldMaxBodySize(values);
}

void Server::addRoute(Field &locationContent) {
    std::string locationTarget = setFieldLocationPath(locationContent.first);
    // TODO: clean_trailing_slash of location and log if so.
    if (_routes.find(locationTarget) == _routes.end())
        _routes[locationTarget] = Route(locationContent.first, locationContent.second, *this);
    else
        warn.log() << "location: " << locationTarget << ": is a redifinition of the location target, input ignored."
                   << std::endl;
}

void Server::addErrorPage(const ValueList &values) {
    HttpCode errorCode = setFieldErrorPageCode(values);
    if (_errorPages.find(errorCode) == _errorPages.end())
        _errorPages[errorCode] = setFieldErrorPagePath(values);
    else
        warn.log() << "error_page: " << errorCode << ": is a redifinition of the error page, input ignored."
                   << std::endl;
}
