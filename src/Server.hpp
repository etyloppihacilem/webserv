#ifndef __INCLUDE_SRC_SERVER_HPP_
#define __INCLUDE_SRC_SERVER_HPP_

#include <string>
#include <map>

#include "HttpMethods.hpp"
#include "Route.hpp"

	// unsigned int _maxBodySize;
	// unsigned int _maxConnection;

class Server {

private:
	std::vector<std::string> _serverName;
	unsigned int _port;
	bool _autoindex;
	std::vector<HttpMethod > _methods;
	std::string _rootDir;
	std::vector<std::string> _indexPage;
	std::map<std::string, Route> _routes;
	std::map<std::string, std::string> _errorPages;

public:
	Server();
	//Basic Server constructor
	Server(std::string ipAddress = "127.0.0.1", 
			unsigned int port = 80, 
			std::string rootDir = "/www/html",
			std::string indexPage = "index.html");
	//conf file Server constructor
	Server(std::string);
	~Server();

	std::vector<std::string> getServerName() const;
	unsigned int getPort() const;
	bool getAutoindex() const;
	std::vector<HttpMethod> getMethods() const;
	std::string getRootDir() const;
	std::vector<std::string> getIndexPage() const;
	std::map<std::string, std::string> getErrorPages() const;
	bool hasRoute(const std::string &path) const;
	Route &findRoute(const std::string &path);
	Route *getRoute(const std::string &path);
	bool hasServeName(const std::string &serverName) const;

};

#endif // !__INCLUDE_SRC_SERVER__
