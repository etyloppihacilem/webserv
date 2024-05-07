#ifndef __INCLUDE_SRC_SERVER__
#define __INCLUDE_SRC_SERVER__

#include <string>
#include <map>

#include "Socket.hpp"
#include "Route.hpp"
#include "HttpMethods.hpp"

class Server {

private:
	std::vector<std::string> _serverName;
	unsigned int _port;
	bool _autoindex;
	std::vector<int> _methods;
	std::string _rootDir;
	std::vector<std::string> _indexPage;
	std::map<std::string, Route> _routes;
	std::map<std::string, std::string> _errorPages;

	// std::string _ipAddress;
	// unsigned int _maxBodySize;
	// unsigned int _maxConnection;

public:
	Server();
	//Basic Server constructor
	Server(std::string ipAddress, unsigned int port, std::string rootDir, std::string indexPage);
	//conf file Server constructor
	Server(std::string);
	~Server();

	std::vector<std::string> getServerName() const;
	unsigned int getPort() const;
	bool getAutoindex() const;
	std::vector<int> getMethods() const;
	std::string getRootDir() const;
	std::string getIndexPage() const;
	std::map<std::string, std::string> getErrorPages() const;
	bool hasRoute(const std::string &path) const;
	Route &getRoute(const std::string &path) const;
	bool hasServeName(const std::string &serverName) const;

};

#endif // !__INCLUDE_SRC_SERVER__
