#include "Server.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "Route.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfLogging.hpp"
#include "ServerConfSetter.hpp"
#include "ServerConfTokenize.hpp"
#include "ServerGetRoute.hpp"
#include "StringTokenizer.hpp"
#include "StringUtils.hpp"
#include <algorithm>
#include <cstddef>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

Server::Setter Server::fieldSetterList[COUNT_CONF_FIELD] = {
    0,
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
    0,
};

HttpMethod Server::initMethod[1] = { GET };

Server::Server() :
    ServerGetRoute(),
    _serverName(1, "localhost"),
    _port(8080),
    _rootDir("www"),
    _indexPage(1, "index.html"),
    _autoindex(false),
    _methods(initMethod, initMethod + 1),
    _maxBodySize(MAX_BODY_SIZE),
    _errorPages(),
    _isFieldSet(COUNT_CONF_FIELD, false) {
    _routes["/"] = Route(*this);
}

Server::Server(StringTokenizer &tokenizedServer) :
    ServerGetRoute(),
    _serverName(1, "localhost"),
    _port(8080),
    _rootDir("www"),
    _indexPage(),
    _autoindex(false),
    _methods(initMethod, initMethod + 1),
    _maxBodySize(MAX_BODY_SIZE),
    _errorPages(),
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
        addRoute(*it);
    if (!hasRoute("/"))
        _routes["/"] = Route(*this);

    logParsingEnded(server);
}

Server::~Server() {}

const std::vector< std::string > &Server::getServerName() const {
    return _serverName;
}

int Server::getPort() const {
    return _port;
}

const std::string &Server::getRootDir() const {
    return _rootDir;
}

const std::vector< std::string > &Server::getIndexPage() const {
    return _indexPage;
}

bool Server::getAutoindex() const {
    return _autoindex;
}

const std::set< HttpMethod > &Server::getMethods() const {
    return _methods;
}

std::size_t Server::getMaxBodySize() const {
    return _maxBodySize;
}

const std::map< HttpCode, std::string > &Server::getErrorPages() const {
    return _errorPages;
}

bool Server::hasServeName(const std::string &serverName) const {
    std::vector< std::string >::const_iterator name = _serverName.end();

    name = std::find(_serverName.begin(), _serverName.end(), serverName);
    if (name == _serverName.end())
        return false;
    else
        return true;
}

bool Server::hasServeNameSet() const {
    return _isFieldSet[server_name];
}

bool Server::hasListenSet() const {
    return _isFieldSet[listen_f];
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
    if (_routes.find(locationTarget) == _routes.end())
        _routes[locationTarget] = Route(locationContent.first, locationContent.second, *this);
    else
        warn.log() << "location: " << locationTarget << ": is a redifinition of the location target, input ignored."
                   << std::endl;
}

void Server::addErrorPage(const ValueList &values) {
    HttpCode errorCode = setFieldErrorPageCode(values);
    if (_errorPages.find(errorCode) == _errorPages.end())
        _errorPages[errorCode] = values[1];
    else
        warn.log() << "error_page: " << errorCode << ": is a redifinition of the error page, input ignored."
                   << std::endl;
}
