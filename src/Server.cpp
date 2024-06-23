#include "Server.hpp"
#include "HttpMethods.hpp"
#include "StringTokenizer.hpp"
#include "ServerConfTokenize.hpp"
#include "ServerConfValidate.hpp"
#include "Logger.hpp"
#include <sstream>
#include <algorithm>

Server::Server():
    _serverName (),
    _port       (8080),
    _autoindex  (true),
    _methods    (1),
    _rootDir    ("./"),
    _indexPage  (),
    _maxBodySize(1000000),
    _routes     (),
    _errorPages () {
    _serverName.push_back("localhost");
    _indexPage.push_back("index.html");

    _routes["./"] = Route();

    _errorPages[BadRequest]                 = "error_pages/400.html";
    _errorPages[Forbidden]                  = "error_pages/403.html";
    _errorPages[NotFound]                   = "error_pages/404.html";
    _errorPages[MethodNotAllowed]           = "error_pages/405.html";
    _errorPages[ContentTooLarge]            = "error_pages/413.html";
    _errorPages[InternalServerError]        = "error_pages/500.html";
    _errorPages[NotImplemented]             = "error_pages/501.html";
    _errorPages[HTTPVersionNotSupported]    = "error_pages/505.html";
}

int findFieldCode(const std::string &fieldName)
{
    for (int i = 0; i < COUNT_CONF_FIELD; ++i) {
        if (fieldName == ConfFieldString(i)) {
            return i;
        }
    }
    return -1;
}

Server::Server(const std::string &serverContent) {
    StringTokenizer tokenizedServer(serverContent, "|");

    std::replace(serverContent.begin(), serverContent.end(), "|", " ");
    info.log() << "server: " + serverContent.substr(0, 30)
        + " ... : parsing started..." << std::endl;
    while (tokenizedServer.hasMoreTokens()) {
        try {
            Field fieldContent;

            if (tokenizedServer.peakToken() == ConfFieldString(location)) {
                fieldContent = tokenizeLocation(tokenizedServer);
                if (!isValidLocation(fieldContent.second)) {
                    std::replace(fieldContent.second.begin(), fieldContent.second.end(), "|", " ");
                    warn.log()
                        << "location " + fieldContent.first + " : " + tokenizedServer.remainingString().substr(0, 30)
                        + " ... : this 'location' module does not possess mandatory fields, parsing canceled"
                        << std::endl;
                }
            } else {
                fieldContent = tokenizeField(tokenizedServer);
            }

            int         fieldCode       = findFieldCode(fieldContent.first);
            ValueList   valueContent    = tokenizeValue(fieldContent.second);

            switch (fieldCode) {
                case 2:
                    this->addRoute(fieldContent);
                    break;
                case 3:
                    this->addErrorPage(valueContent);
                    break;
                case 4:
                    this->setServerName(valueContent);
                    break;
                case 5:
                    this->setPort(valueContent);
                    break;
                case 6:
                    this->setRootDir(valueContent);
                    break;
                case 7:
                    this->setIndexPage(valueContent);
                    break;
                case 8:
                    this->setAutoindex(valueContent);
                    break;
                case 9:
                    this->setMaxBodySize(valueContent);
                    break;
                default:
                    warn.log() << fieldContent.first + ": is not a valide Server Conf Field" << std::endl;
                    throw ServerConfWarn();
            }
        } catch (ServerConfWarn &e) {
            continue;
        } catch (ServerConfError &e) {
            throw;
        }
    }
}

Server::~Server() {}

std::vector<std::string> Server::getServerName() const{
    return _serverName;
}

unsigned int Server::getPort() const{
    return _port;
}

bool Server::getAutoindex() const{
    return _autoindex;
}

std::vector<HttpMethod> Server::getMethods() const{
    return _methods;
}

std::string Server::getRootDir() const{
    return _rootDir;
}

std::vector<std::string> Server::getIndexPage() const{
    return _indexPage;
}

std::map<HttpCode, std::string> Server::getErrorPages() const{
    return _errorPages;
}

bool Server::hasRoute(const std::string &path) const{
    return _routes.find(path) == _routes.end() ? false : true;
}

Route &Server::getRoute(const std::string &path)
{
    if (hasRoute(path)) {
        return _routes[path];
    }
    throw RouteNotFoundWarn(path);
}

bool Server::hasServeName(const std::string &serverName) const{
    std::vector<std::string>::const_iterator name = _serverName.end();

    name = std::find(_serverName.begin(), _serverName.end(), serverName);
    if (name == _serverName.end()) {
        return false;
    } else {
        return true;
    }
}

void Server::addRoute(const Field &locationContent)
{
    _routes[locationContent.first] = Route(locationContent.second);
}

void Server::addErrorPage(const ValueList &valueContent)
{
    if (valueContent.size() != 2) {
        warn.log() << "fail to parse 'error_page', accept only a error code and a path value." << std::endl;
        throw ServerConfWarn();
    }

    std::stringstream   errorStr(valueContent[0]);
    int                 errorCode;

    if (errorStr >> errorCode) {
        error.log() << "error_page: " << valueContent[0] << ": is not a valid int." << std::endl;
        throw ServerConfError();
    }
    if (!isError(errorCode)) {
        error.log() << "error_page: " << valueContent[0] << ": is not a valid Http error." << std::endl;
        throw ServerConfError();
    }
    if (errorCode > 599) {
        info.log()  << "error_page: " << valueContent[0] << ": this error code is not standard because above 599."
                    << std::endl;
    }
    if (!isValidPath(valueContent[1])) {
        warn.log()  << "error_page: " << valueContent[0] << " " << valueContent[1] << "is not a valid path."
                    << std::endl;
    }
    _errorPages[HttpCode(errorCode)] = valueContent[1];
}

void Server::setServerName(const ValueList &valueContent)
{
    for (ValueList::const_iterator it = valueContent.begin(); it < valueContent.end(); ++it) {
        if (!isValidIPAddress(*it)) {
            if (!isValidHostName(*it)) {
                warn.log() << "server_name: " << *it << ": is not a valid Hostname." << std::endl;
            }
            warn.log() << "server_name: " << *it << ": is not a a valid IP address." << std::endl;
        }
        _serverName.push_back(*it);
    }
    if (_serverName.empty()) {
        error.log() << "The server does not possess any valid server name, parsing canceled." << std::endl;
        throw ServerConfError();
    }
}

void Server::setPort(const ValueList &valueContent)
{
    if (valueContent.size() != 1) {
        error.log() << "fail to parse 'listen', accept only one value." << std::endl;
        throw ServerConfError();
    }

    std::stringstream   port(valueContent[0]);
    int                 portNumber;

    if (port >> portNumber) {
        error.log() << "port: " << valueContent[0] << ": is not a valid int." << std::endl;
        throw ServerConfError();
    }
    if (!isValidPort(portNumber)) {
        error.log() << "port: " << valueContent[0] << ": is not a valid port number." << std::endl;
        throw ServerConfError();
    }
    _port = portNumber;
}

void Server::setRootDir(const ValueList &valueContent)
{
    if (valueContent.size() != 1) {
        error.log() << "fail to parse 'root', accept only one value." << std::endl;
        throw ServerConfError();
    }
    if (!isValidRoot(valueContent[0])) {
        error.log() << "root: " << valueContent[0] << ": is not a valid root." << std::endl;
    }
    _rootDir = valueContent[0];
}

void Server::setIndexPage(const ValueList &valueContent)
{
    for (ValueList::const_iterator it = valueContent.begin(); it < valueContent.end(); ++it) {
        if (!isValidIndex(*it)) {
            warn.log() << "index: " << *it << ": is not a valid index page." << std::endl;
            throw ServerConfWarn();
        }
        _indexPage.push_back(*it);
    }
}

void Server::setAutoindex(const ValueList &valueContent)
{
    if (valueContent.size() != 1) {
        error.log() << "fail to parse 'autoindex', accept only one value." << std::endl;
        throw ServerConfError();
    }

    if (!isValidAutoindex(valueContent[0]))
    {
    	warn.log() << "autoindex: " << valueContent[0] << ": is not a valid bool." << std::endl;
    }
	if (valueContent[0] == "true")
	{
		_autoindex = true;
	}
	else {
		_autoindex = false;
	}
}

void Server::setMaxBodySize(const ValueList &valueContent)
{
    if (valueContent.size() != 1) {
        error.log() << "fail to parse 'max_body_size', accept only one value." << std::endl;
        throw ServerConfError();
    }

    std::stringstream   maxBodySizeStr(valueContent[0]);
    int                 maxBodySizeNumber;

    if (maxBodySizeStr >> maxBodySizeNumber) {
        error.log() << "max_body_size: " << valueContent[0] << ": is not a valid int." << std::endl;
        throw ServerConfError();
    }
    if (!isValidPort(maxBodySizeNumber)) {
        error.log() << "max_body_size: " << valueContent[0] << ": is not a valid max_body_size number." << std::endl;
        throw ServerConfError();
    }
    _port = maxBodySizeNumber;
}
