#include "ServerManager.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfTokenize.hpp"
#include "ServerConfValidate.hpp"
#include "StringTokenizer.hpp"
#include <exception>
#include <fstream>
#include <sstream>
#include <algorithm>

ServerManager *ServerManager::_instance = nullptr;

ServerManager *ServerManager::getInstance(const std::string &configFile)
{
    if (_instance == nullptr) {
        try {
            _instance = new ServerManager(configFile);
        } catch (std::exception &e) {
            _instance = nullptr;
        }
    }
    return _instance;
}

void ServerManager::deleteInstance()
{
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

ServerManager::~ServerManager() {}

std::string readConfFile(std::ifstream &configStream)
{
    std::string buff;
    std::string fileContent;

    while (std::getline(configStream, buff)) {
        if (buff.empty()) {
            continue;
        }
        fileContent += buff;
    }
    return fileContent;
}

ServerManager::ServerManager(const std::string &configFile) {
    try {
        if (!isValidConfigFile(configFile)) {
            throw ServerConfError(configFile + ": file is not in the expected format [ *.conf ].");
        }

        std::ifstream configStream(configFile);

        if (configStream.fail()) {
            throw ServerConfError(configFile + ": fail to open file.");
        }

        std::string fileContent = tokenizeFile(readConfFile(configStream));

        if (!isValidHttp(fileContent)) {
            std::replace(fileContent.begin(), fileContent.end(), "|", " ");
            throw ServerConfError(fileContent.substr(0, 30)
                    + " ... : this 'http' module does not possess at least one 'server' module, parsing canceled");
        }

        StringTokenizer tokenizedFile(fileContent, "|");

        while (tokenizedFile.hasMoreTokens()) {
            std::string serverContent = tokenizeServer(tokenizedFile);

            if (!isValidServer(serverContent)) {
                std::replace(serverContent.begin(), serverContent.end(), "|", " ");
                warn.log(std::string("server: " + serverContent.substr(0, 30)
                        + " ... : this 'server' module does not possess mandatory fields, parsing canceled").c_str());
                continue;
            }

            Server newServer(serverContent);
        }
        if (_servers.empty()) {
            throw ServerConfError(configFile + ": no valid server child in config file.");
        }
    } catch (ServerConfError &e) {
        throw FailToInitServerError();
    }
    // TODO start Reactor from here
}

Server &ServerManager::getServer(const std::string &serverName, int port)
{
    for (std::vector<Server>::iterator it = _servers.begin(); it < _servers.end(); ++it) {
        if (it->hasServeName(serverName) && it->getPort() == static_cast<unsigned int>(port)) {
            return *it;
        }
    }

    std::stringstream   portStr;

    portStr << port;

    throw ServerNotFoundWarn(serverName + portStr.str());
}
