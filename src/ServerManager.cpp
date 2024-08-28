#include "ServerManager.hpp"
#include "EventHandler.hpp"
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
#include <regex.h>
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

std::vector< Server > ServerManager::parseConfFile(const std::string &configFile) {
    std::vector< Server > servers;
    if (configFile.empty())
        return servers;

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
            servers.push_back(newServer);
        } catch (ServerConfError &e) {
            throw FailToInitServerError();
        }
    }
    return servers;
}

ServerManager::ServerManager(const std::string &configFile) : _servers(parseConfFile(configFile)), _reactor(_servers) {}

ServerManager::~ServerManager() {
    deleteInstance();
}

void ServerManager::addClient(int socket_fd, int port) {
    _reactor.addClient(socket_fd, port);
}

void ServerManager::deleteClient(int socket_fd) {
    _reactor.deleteClient(socket_fd);
}

void ServerManager::listenToClient(int socket_fd, EventHandler &handler) {
    _reactor.listenToClient(socket_fd, handler);
}

void ServerManager::talkToClient(int socket_fd, EventHandler &handler) {
    _reactor.talkToClient(socket_fd, handler);
}

void ServerManager::run() {
    _reactor.run();
}

Server &ServerManager::getServer(const std::string &serverName, int port) {
    for (std::vector< Server >::iterator it = _servers.begin(); it != _servers.end(); ++it)
        if (it->hasServeName(serverName) && it->getPort() == port)
            return *it;

    std::stringstream portStr;
    portStr << port;
    throw ServerNotFoundWarn(serverName + portStr.str());
}

Server &ServerManager::getServer(int port) {
    for (std::vector< Server >::iterator it = _servers.begin(); it != _servers.end(); ++it)
        if (it->getPort() == port)
            return *it;

    std::stringstream portStr;
    portStr << port;
    throw ServerNotFoundWarn(portStr.str());
}
