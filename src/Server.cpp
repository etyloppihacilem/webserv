#include "Server.hpp"
#include "HttpMethods.hpp"
#include "StringTokenizer.hpp"
#include "ServerConfTokenize.hpp"
#include "ServerConfValidate.hpp"
#include <cctype>
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

    while (tokenizedServer.hasMoreTokens()) {
        try {
            Field fieldContent;

            if (tokenizedServer.peakToken() == ConfFieldString(location)) {
                fieldContent = tokenizeLocation(tokenizedServer);
                if (!isValidLocation(fieldContent.second)) {
                    std::replace(fieldContent.second.begin(), fieldContent.second.end(), "|", " ");
                    warn.log(std::string("location " + fieldContent.first + " : "
                            + tokenizedServer.remainingString().substr(0, 30)
                            + " ... : this 'location' module does not possess mandatory fields, parsing canceled").c_str());
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
                    throw ServerConfWarn(fieldContent.first + ": is not a valide Server Conf Field");
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

// Route &Server::findRoute(const std::string &path)
// {
//  return _routes[path];
//  return _routes.find(path) == _routes.end() ? 0 : &_routes.find(path)->second;
// }

Route &Server::getRoute(const std::string &path)
{
    return _routes[path];
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
    if (isValidErrorPage(valueContent)) {
        std::stringstream   error(valueContent[0]);
        int                 errorCode;

        error >> errorCode;
        _errorPages[HttpCode(errorCode)] = valueContent[1];
        return ;
    }
    warn.log(std::string(valueContent[0] + ": is not a valid error page").c_str());
}

void Server::setServerName(const ValueList &valueContent)
{
    for (ValueList::const_iterator it = valueContent.begin(); it < valueContent.end(); ++it) {
    	if (!isValidIPAddress(*it) || !isValidHostName(*it))
    	{
    		warn.log(std::string(*it + ": is not a valid server name.").c_str());
    	}
    	_serverName.push_back(*it);
    }
}

void Server::setPort(const ValueList &valueContent)
{
	if (valueContent.size() != 1 || valueContent[0].empty()) {
		return ;
	}
	std::stringstream port(valueContent[0]);
	int portNumber;
	if (port >> portNumber) {
		warn.log(std::string(valueContent[0] + ": is not a valid int.").c_str());
	}
	if (!isValidPort(portNumber))
	{
		warn.log(std::string(valueContent[0] + ": is not a valid port number.").c_str());
	}
	_port = portNumber;
}
