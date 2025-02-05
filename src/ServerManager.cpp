#include "ServerManager.hpp"
#include "EventHandler.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfLogging.hpp"
#include "ServerConfTokenize.hpp"
#include "ServerReactor.hpp"
#include "StringTokenizer.hpp"
#include "StringUtils.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <regex.h>
#include <sstream>
#include <string>
#include <vector>

ServerManager *ServerManager::_instance = 0;
const char   **ServerManager::_env      = 0;

const char **ServerManager::getEnv() {
    if (!_env)
        error.log() << "Getting env but env is null." << std::endl;
    return _env;
}

ServerManager *ServerManager::getInstance(const std::string &configFile, const char **env) {
    if (ServerManager::_instance == 0) {
        ServerManager::_instance = new ServerManager(configFile);
        _env                     = env;
        if (!_env)
            error.log() << "New instance did not set env. Env is null." << std::endl;
    }
    return ServerManager::_instance;
}

void ServerManager::deleteInstance() {
    delete ServerManager::_instance;
    ServerManager::_instance = 0;
}

std::string readConfFile(std::ifstream &configStream) {
    std::string buff;
    std::string fileContent;

    while (std::getline(configStream, buff)) {
        if (buff.empty())
            continue;
        if (buff.find("#") != std::string::npos)
            continue;
        fileContent += " " + buff;
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

bool doesServerExist(const std::vector< Server > &servers, const Server &newServer) {
    for (std::vector< Server >::const_iterator it = servers.begin(); it != servers.end(); ++it) {
        if (it->getPort() == newServer.getPort())
            if (std::equal(
                    newServer.getServerName().begin(), newServer.getServerName().end(), it->getServerName().begin()
                )) {
                logServerRedefinition(newServer.getPort(), newServer.getServerName());
                return true;
            }
    }
    return false;
}

std::vector< Server > ServerManager::parseConfFile(const std::string &configFile) {
    std::vector< Server > servers;
    if (configFile.empty())
        return servers;

    if (extract_extension(configFile) != "conf") {
        error.log() << configFile << ": filenam has not the correct extension [ *.conf ]." << std::endl;
        throw FailToInitServerError();
    }

    std::ifstream configStream(configFile.c_str());
    if (configStream.fail()) {
        error.log() << configFile << ": fail to open config file." << std::endl;
        throw FailToInitServerError();
    }

    try {
        StringTokenizer tokenizedServers = tokenizeFile(cleanConfFile(readConfFile(configStream)));
        while (tokenizedServers.hasMoreTokens()) {
            StringTokenizer tokenizedServer = tokenizeServer(tokenizedServers);
            Server          newServer(tokenizedServer);
            if (!doesServerExist(servers, newServer))
                servers.push_back(newServer);
        }
    } catch (ServerConfError &e) {
        throw FailToInitServerError();
    }
    return servers;
}

ServerManager::ServerManager(const std::string &configFile) : _servers(parseConfFile(configFile)), _reactor(_servers) {}

ServerManager::~ServerManager() {}

int ServerManager::addClient(int socket_fd, int port, std::string client_IP) {
    return _reactor.addClient(socket_fd, port, client_IP);
}

void ServerManager::deleteClient(int socket_fd, EventHandler &handler) {
    _reactor.deleteClient(socket_fd, handler);
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

const Server &ServerManager::getServer(const std::string &serverName, int port) const {
    for (std::vector< Server >::const_iterator it = _servers.begin(); it != _servers.end(); ++it)
        if (it->hasServeName(serverName) && it->getPort() == port)
            return *it;

    std::stringstream portStr;
    portStr << port;
    warn.log() << "Throwing ServerNotFoundWarn with (" << serverName << ", " << portStr.str() << ")" << std::endl;
    throw ServerNotFoundWarn();
}

const Server &ServerManager::getServer(int port) const {
    for (std::vector< Server >::const_iterator it = _servers.begin(); it != _servers.end(); ++it)
        if (it->getPort() == port)
            return *it;

    std::stringstream portStr;
    portStr << port;
    warn.log() << "Throwing ServerNotFoundWarn" << std::endl;
    throw ServerNotFoundWarn(portStr.str());
}
#ifdef TESTING
const ServerReactor &ServerManager::getReactor() const {
    return _reactor;
}
#endif
