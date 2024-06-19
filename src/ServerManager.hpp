#ifndef __INCLUDE_SRC_SERVERMANAGER_HPP_
#define __INCLUDE_SRC_SERVERMANAGER_HPP_

#include <string>
#include <vector>
class Server;

class ServerManager
{
private:
	ServerManager(const std::string& configFile);
	ServerManager(ServerManager &rhs);
	void operator=(const ServerManager &rhs);

	static ServerManager* _instance;
	std::string _configFile;
	std::vector<Server> _servers;

public:
	static ServerManager *getInstance(const std::string &configFile);
	void deleteInstance();
	~ServerManager();

	Server &getServer(const std::string &serverName, int port);
};

#endif // !__INCLUDE_SRC_SERVERMANAGER_
