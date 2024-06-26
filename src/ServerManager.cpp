#include "ServerManager.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfTokenize.hpp"
#include "ServerConfValidate.hpp"
#include "StringTokenizer.hpp"
#include <exception>
#include <fstream>
#include <sstream>
#include <algorithm>

ServerManager *ServerManager::_instance = 0;

ServerManager *ServerManager::getInstance(const std::string &configFile)
{
    if (_instance == 0) {
        try {
            _instance = new ServerManager(configFile);
        } catch (std::exception &e) {
            _instance = 0;
        }
    }
    return _instance;
}

void ServerManager::deleteInstance()
{
    if (_instance != 0) {
        delete _instance;
        _instance = 0;
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
            error.log() << configFile + ": file is not in the expected format [ *.conf ]." << std::endl;
            throw ServerConfError();
        }

        std::ifstream configStream(configFile.c_str());

        if (configStream.fail()) {
            error.log() << configFile + ": fail to open file." << std::endl;
            throw ServerConfError();
        }

        std::string fileContent = tokenizeFile(readConfFile(configStream));

        if (!isValidHttp(fileContent)) {
            std::string errorStr = fileContent.substr(0, 30);

            std::replace(errorStr.begin(), errorStr.end(), '|', ' ');
            error.log() << errorStr
                        << " ... : this 'http' module does not possess at least one 'server' module, parsing canceled"
                        << std::endl;
            throw ServerConfError();
        }

        StringTokenizer tokenizedFile(fileContent, "|");

        while (tokenizedFile.hasMoreTokens()) {
            std::string serverContent = tokenizeServer(tokenizedFile);

            if (!isValidServer(serverContent)) {
                std::string errorStr = serverContent.substr(0, 30);

                std::replace(errorStr.begin(), errorStr.end(), '|', ' ');
                warn.log()  << "server: " << errorStr
                            << " ... : this 'server' module does not possess mandatory fields, parsing canceled"
                            << std::endl;
                continue;
            }

            Server newServer(serverContent);
        }
        if (_servers.empty()) {
            error.log() << configFile + ": no valid server child in config file." << std::endl;
            throw ServerConfError();
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

    std::stringstream portStr;

    portStr << port;

    throw ServerNotFoundWarn(serverName + portStr.str());
}
