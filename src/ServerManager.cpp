#include "ServerManager.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfTokenize.hpp"
#include "StringTokenizer.hpp"
#include "StringUtils.hpp"
#include <algorithm>
#include <exception>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

ServerManager *ServerManager::_instance = 0;

ServerManager *ServerManager::getInstance(const std::string &configFile) {
    if (_instance == 0) {
        try {
            _instance = new ServerManager(configFile);
        } catch (std::exception &e) {
            _instance = 0;
        }
    }
    return _instance;
}

void ServerManager::deleteInstance() {
    if (_instance != 0) {
        delete _instance;
        _instance = 0;
    }
}

ServerManager::~ServerManager() {
    this->deleteInstance();
}

std::string readConfFile(std::ifstream &configStream) {
    std::string buff;
    std::string fileContent;

    while (std::getline(configStream, buff)) {
        if (buff.empty())
            continue;
        if (buff[0] == '#')
            continue;
        fileContent += buff;
    }
    return fileContent;
}

std::string cleanConfFile(std::string fileContent) {
    std::replace(fileContent.begin(), fileContent.end(), ' ', '|');
    std::replace(fileContent.begin(), fileContent.end(), '\t', '|');
    std::replace(fileContent.begin(), fileContent.end(), '\n', '|');
    std::replace(fileContent.begin(), fileContent.end(), '\r', '|');
    return fileContent;
}

ServerManager::ServerManager(const std::string &configFile) {
    if (extract_extension(configFile) != "conf") {
        error.log() << configFile << ": file is not in the expected format [ *.conf ]." << std::endl;
        throw FailToInitServerError();
    }

    std::ifstream configStream(configFile.c_str());
    if (configStream.fail()) {
        error.log() << configFile << ": fail to open config file." << std::endl;
        throw FailToInitServerError();
    }

    StringTokenizer tokenizedServers = tokenizeFile(cleanConfFile(readConfFile(configStream)));
    while (tokenizedServers.hasMoreTokens()) {
        try {
            StringTokenizer tokenizedServer = tokenizeServer(tokenizedServers);
            Server          newServer(tokenizedServer);
            // TODO: check that server cannot use hostname and port twice.
            _servers.push_back(newServer);
        } catch (ServerConfError &e) {
            throw FailToInitServerError();
        }
    }
    // TODO start Reactor from here
}

Server &ServerManager::getServer(const std::string &serverName, int port) {
    for (std::vector< Server >::iterator it = _servers.begin(); it < _servers.end(); ++it)
        if (it->hasServeName(serverName) && it->getPort() == port)
            return *it;

    std::stringstream portStr;
    portStr << port;
    throw ServerNotFoundWarn(serverName + portStr.str());
}
