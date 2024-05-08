#include "Server.hpp"
#include "HttpMethods.hpp"

#include <algorithm>

Server::Server() 
{
}

Server::Server(std::string serverName, unsigned int port, std::string root, std::string index) :
	_serverName(1),
	_port(port),
	_autoindex(false),
	_methods(1),
	_rootDir(root),
	_indexPage(1),
	_routes(),
	_errorPages()
{
	_serverName.push_back(serverName);
	_methods.push_back(GET);
	_indexPage.push_back(index);
}

Server::~Server() 
{
}

std::vector<std::string> Server::getServerName() const 
{
	return _serverName;
}

unsigned int Server::getPort() const
{
	return _port;
}

bool Server::getAutoindex() const
{
	return _autoindex;
}

std::vector<HttpMethod> Server::getMethods() const
{
	return _methods;
}

std::string Server::getRootDir() const
{
	return _rootDir;
}

std::vector<std::string> Server::getIndexPage() const
{
	return _indexPage;
}

std::map<std::string, std::string> Server::getErrorPages() const
{
	return _errorPages;
}

bool Server::hasRoute(const std::string &path) const
{
	return _routes.find(path) == _routes.end() ? false : true;
}

Route &Server::findRoute(const std::string &path)
{
	return _routes[path];
}

Route *Server::getRoute(const std::string &path)
{
	return _routes.find(path) == _routes.end() ? 0 : &_routes.find(path)->second;
}

bool Server::hasServeName(const std::string &serverName) const
{
	std::vector<std::string>::const_iterator name = _serverName.end();
	name = std::find(_serverName.begin(), _serverName.end(), serverName);
	if (name ==  _serverName.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}
