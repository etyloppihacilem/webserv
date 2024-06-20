#include "ServerManager.hpp"
#include "ServerConfTokenize.hpp"
#include "ServerConfValidate.hpp"
#include "StringTokenizer.hpp"
#include <exception>
#include <fstream>
#include <cstdlib>

ServerManager *ServerManager::_instance = nullptr;

ServerManager *ServerManager::getInstance(const std::string &configFile)
{
	if (_instance == nullptr)
	{
		try{
			_instance = new ServerManager(configFile);
		}
		catch(std::exception &e)
		{
			_instance = nullptr;
		}
	}
	return _instance;
}

void ServerManager::deleteInstance()
{
	if (_instance != nullptr)
	{
		delete _instance;
		_instance = nullptr;
	}
}

ServerManager::~ServerManager()
{
}

std::string readConfFile(std::ifstream &configStream)
{
	std::string buff;
	std::string fileContent;
	while (std::getline(configStream, buff))
	{
		if (buff.empty())
		{
			continue ;
		}
		fileContent += buff;
	}
	return fileContent;
}

ServerManager::ServerManager(const std::string &configFile)
{
	if (!isValidConfigFile(configFile))
	{
		throw(ConfError(configFile + ": file is not in the expected format [ *.conf ]."));
	}
	std::ifstream configStream(configFile);
	if (configStream.fail())
	{
		throw(ConfError(configFile + ": fail to open file."));
	}
	std::string fileContent = tokenizeFile(readConfFile(configStream));
	if (fileContent.empty() || !isValidHttp(fileContent))
	{
		throw(ConfError(configFile + ": no file root in config file."));
	}
	StringTokenizer tokenizedFile(fileContent, "|");
	std::vector<std::string> tokenizedServers;
	while (tokenizedFile.hasMoreTokens())
	{
		std::string serverContent = tokenizeServer(tokenizedFile);
		if (isValidServer(serverContent))
		{
			try {
				Server newServer(serverContent);
			} catch (std::exception &e) {
				continue ;
			}
		}
	}
	if (_servers.empty())
	{
		throw(ConfError(configFile + ": no valid server child in config file."));
	}
	// TODO start Reactor from here
}

Server &ServerManager::getServer(const std::string &serverName, int port)
{
	for (std::vector<Server>::iterator it = _servers.begin(); it < _servers.end(); ++it)
	{
		if (it->hasServeName(serverName) && it->getPort() == static_cast<unsigned int>(port))
		{
			return *it;
		}
	}
	char *str = itoa(port);
	throw(ServerNotFoundWarn(serverName + std::string(std::itoa(port))));
}
