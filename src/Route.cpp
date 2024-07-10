#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "Route.hpp"
#include "Server.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfTokenize.hpp"
#include "ServerConfValidate.hpp"
#include "StringTokenizer.hpp"
#include <algorithm>
#include <cstddef>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

Route::Route() {}

Route::Route(Server &server):
    _methods        (server.getMethods()),
    _rootDir        (server.getRootDir()),
    _indexPage      (server.getIndexPage()),
    _autoindex      (server.getAutoindex()),
    _uploadPath     (""),
    _redirCode      (""),
    _redirPage      (""),
    _cgiPath        (""),
    _cgiExtension   (""),
    _indexPageSet   (false),
    _autoindexSet   (false),
    _rootDirSet     (false),
    _methodsSet     (false),
    _redirectionSet (false),
    _uploadSet      (false),
    _cgiPathSet     (false),
    _cgiExtensionSet(false) {}

Route::Route(const std::string &locationContent, Server &server):
    _methods        (server.getMethods()),
    _rootDir        (server.getRootDir()),
    _indexPage      (server.getIndexPage()),
    _autoindex      (server.getAutoindex()),
    _uploadPath     (""),
    _redirCode      (""),
    _redirPage      (""),
    _cgiPath        (""),
    _cgiExtension   (""),
    _indexPageSet   (false),
    _autoindexSet   (false),
    _rootDirSet     (false),
    _methodsSet     (false),
    _redirectionSet (false),
    _uploadSet      (false),
    _cgiPathSet     (false),
    _cgiExtensionSet(false) {
    StringTokenizer tokenizedLocation(locationContent, "|");
    ValueList       tokenizedUpload;
    ValueList       tokenizedRedirection;
    ValueList       tokenizedCgiPath;
    ValueList       tokenizedFileExt;

    std::string infoStr = locationContent.substr(0, 30);

    std::replace(infoStr.begin(), infoStr.end(), '|', ' ');
    info.log() << "location: " << infoStr << " ... : parsing started..." << std::endl;
    while (tokenizedLocation.hasMoreTokens()) {
        try {
            Field fieldContent;

            fieldContent = tokenizeField(tokenizedLocation);

            int         fieldCode       = findFieldCode(fieldContent.first);
            ValueList   valueContent    = tokenizeValue(fieldContent.second);

            switch (fieldCode) {
                case 6:
                    if (_rootDirSet) {
                        warn.log()  << "root: " << valueContent[0]
                                    << ": is a redifinition of the current value, field ignored." << std::endl;
                        break;
                    }
                    this->setRootDir(valueContent);
                    break;
                case 7:
                    if (_indexPageSet) {
                        warn.log()  << "index: " << valueContent[0]
                                    << ": is a redefinition of the value, field ignored." << std::endl;
                        break;
                    }
                    this->setIndexPage(valueContent);
                    break;
                case 8:
                    if (_autoindexSet) {
                        warn.log()  << "autoindex: " << valueContent[0]
                                    << ": is a redifinition of the current value, field ignored." << std::endl;
                        break;
                    }
                    this->setAutoindex(valueContent);
                    break;
                case 9:
                    if (_methodsSet) {
                        warn.log() << "methods: ";
                        for (size_t i = 0; i < valueContent.size(); ++i) {
                            warn.log() << valueContent[i] << " ";
                        }
                        warn.log() << "is a redifinition of the current value, field ignored." << std::endl;
                        break;
                    }
                    this->setMethods(valueContent);
                    break;
                case 11:
                    if (_redirectionSet) {
                        warn.log()  << "rewrite: " << valueContent[0]
                                    << " is a redifinition of the current value, field ignored." << std::endl;
                        break;
                    }
                    tokenizedRedirection    = valueContent;
                    _redirectionSet         = true;
                    break;
                case 12:
                    if (_uploadSet) {
                        warn.log()  << "upload_path: " << valueContent[0]
                                    << " is a redifinition of the current value, field ignored." << std::endl;
                        break;
                    }
                    tokenizedUpload = valueContent;
                    _uploadSet      = true;
                    break;
                case 13:
                    if (_cgiPathSet) {
                        warn.log()  << "cgi_path: " << valueContent[0]
                                    << " is a redifinition of the current value, field ignored." << std::endl;
                        break;
                    }
                    tokenizedCgiPath    = valueContent;
                    _cgiPathSet         = true;
                    break;
                case 14:
                    if (_cgiExtensionSet) {
                        warn.log()  << "file_ext: " << valueContent[0]
                                    << " is a redifinition of the current value, field ignored." << std::endl;
                        break;
                    }
                    tokenizedFileExt    = valueContent;
                    _cgiExtensionSet    = true;
                    break;
                default:
                    warn.log() << fieldContent.first << ": is not a valide Server Conf Field" << std::endl;
                    throw ServerConfWarn();
            }
        } catch (ServerConfWarn &e) {
            continue;
        }
        try {
            this->setRedirection(tokenizedRedirection);
            this->setUpload(tokenizedUpload);
            this->setCgi(tokenizedCgiPath, tokenizedFileExt);
        } catch (ServerConfWarn &e) {
            ;
        }
    }
}

Route::~Route() {}

bool Route::getAutoindex() const{
    return _autoindex;
}

std::vector<HttpMethod> Route::getMethods() const{
    return _methods;
}

std::vector<std::string> Route::getIndexPage() const{
    return _indexPage;
}

std::string Route::getUploadPath() const{
    return _uploadPath;
}

std::string Route::getRedirCode() const{
    return _redirCode;
}

std::string Route::getRedirPage() const{
    return _redirPage;
}

std::string Route::getCgiPath() const{
    return _cgiPath;
}

std::string Route::getCgiExtension() const{
    return _cgiExtension;
}

bool Route::hasAutoindex() const{
    return _autoindexSet;
}

bool Route::hasMethods() const{
    return _methodsSet;
}

bool Route::hasRoot() const{
    return _rootDirSet;
}

bool Route::hasIndexPage() const{
    return _indexPageSet;
}

bool Route::hasRedir() const{
    return _redirectionSet;
}

bool Route::hasUpload() const{
    return _uploadSet;
}

bool Route::hasCgiPath() const{
    return _cgiPathSet;
}

bool Route::hasCgiExtension() const{
    return _cgiExtensionSet;
}

void Route::setRootDir(const ValueList &valueContent)
{
    _rootDirSet = true;
    if (valueContent.size() != 1) {
        warn.log() << "root: ";
        for (size_t i = 0; i < valueContent.size(); ++i) {
            warn.log() << valueContent[i] << " ";
        }
        warn.log() << ", fail to parse field, it accept only one value." << std::endl;
        throw ServerConfWarn();
    }
    if (!isValidRoot(valueContent[0])) {
        warn.log() << "root: " << valueContent[0] << " is not a valid root." << std::endl;
        throw ServerConfWarn();
    }
    _rootDir = valueContent[0];
}

void Route::setIndexPage(const ValueList &valueContent)
{
    _indexPageSet = true;
    for (ValueList::const_iterator it = valueContent.begin(); it < valueContent.end(); ++it) {
        if (!isValidIndex(*it, _rootDir)) {
            warn.log() << "index: " << *it << " is not a valid index page." << std::endl;
            continue;
        }
        _indexPage.push_back(*it);
    }
}

void Route::setAutoindex(const ValueList &valueContent)
{
    _autoindexSet = true;
    if (valueContent.size() != 1) {
        warn.log() << "autoindex: ";
        for (size_t i = 0; i < valueContent.size(); ++i) {
            warn.log() << valueContent[i] << " ";
        }
        warn.log() << ", fail to parse field, it accept only one value." << std::endl;
        throw ServerConfWarn();
    }
    if (!isValidAutoindex(valueContent[0])) {
        warn.log() << "autoindex: " << valueContent[0] << " is not a valid bool." << std::endl;
        throw ServerConfWarn();
    }
    if (valueContent[0] == "true") {
        _autoindex = true;
    } else {
        _autoindex = false;
    }
}

void Route::setMethods(const ValueList &valueContent)
{
    _methodsSet = true;
    for (ValueList::const_iterator it = valueContent.begin(); it < valueContent.end(); ++it) {
        if (!isValidMethods(*it)) {
            warn.log() << "methods: " << *it << " is not a valid Http method." << std::endl;
            continue;
        }
        if (*it == "GET") {
            _methods.push_back(GET);
        }
        if (*it == "POST") {
            _methods.push_back(POST);
        }
        if (*it == "DELETE") {
            _methods.push_back(DELETE);
        }
    }
}

void Route::setUpload(const ValueList &valueContent) {
    _uploadSet = true;
    // mandatory POST method and upload_path valid
    if (valueContent.size() != 1) {
        warn.log() << "listen: ";
        for (size_t i = 0; i < valueContent.size(); ++i) {
            warn.log() << valueContent[i] << " ";
        }
        warn.log() << ", fail to parse field, it accept only one value." << std::endl;
        throw ServerConfWarn();
    }
    if (!isValidPath(valueContent[0], _rootDir)) {
        warn.log() << "upload_path: " << valueContent[0] << " is not a valid path." << std::endl;
        throw ServerConfWarn();
    }
    if (std::find(_methods.begin(), _methods.end(), POST) == _methods.end()) {
        warn.log() << "upload_path: " << valueContent[0] << " can not be set as POST method is missing." << std::endl;
        throw ServerConfWarn();
    }
    _uploadPath = valueContent[0];
}

void Route::setRedirection(const ValueList &valueContent) {
    _redirectionSet = true;
    if (valueContent.size() != 2) {
        warn.log() << "rewrite: ";
        for (size_t i = 0; i < valueContent.size(); ++i) {
            warn.log() << valueContent[i] << " ";
        }
        warn.log() << ", fail to parse field, it accept only a redir code and a path value." << std::endl;
        throw ServerConfWarn();
    }

    std::stringstream   redirCodeStr(valueContent[0]);
    int                 redirCode;

    if (redirCodeStr >> redirCode) {
        warn.log() << "rewrite: " << valueContent[0] << " is not a valid int." << std::endl;
        throw ServerConfWarn();
    }
    if (!isRedirection(redirCode)) {
        warn.log() << "rewrite: " << valueContent[0] << " is not a valid Http redir." << std::endl;
        throw ServerConfWarn();
    }
    if (!isValidUrl(valueContent[1])) {
        warn.log()  << "rewrite: " << valueContent[0] << " " << valueContent[1] << " " << valueContent[1]
                    << " is not a valid path." << std::endl;
        throw ServerConfWarn();
    }
    _redirCode  = redirCode;
    _redirPage  = valueContent[1];
}

void Route::setCgi(const ValueList &cgiPathContent, const ValueList &cgiExtensionContent)
{
    this->setCgiPath(cgiPathContent);
    this->setCgiExtension(cgiExtensionContent);
}

void Route::setCgiPath(const ValueList &valueContent) {
    _cgiPathSet = true;
    if (valueContent.size() != 1) {
        warn.log() << "cgi_path: ";
        for (size_t i = 0; i < valueContent.size(); ++i) {
            warn.log() << valueContent[i] << " ";
        }
        warn.log() << ", fail to parse field, it accept only one value." << std::endl;
        throw ServerConfWarn();
    }
    if (!isValidPath(valueContent[0], _rootDir)) {
        warn.log() << "cgi_path: " << valueContent[0] << " is not a valid path." << std::endl;
        throw ServerConfWarn();
    }
    _cgiPath = valueContent[0];
}

void Route::setCgiExtension(const ValueList &valueContent) {
    _cgiExtensionSet = true;
    if (valueContent.size() != 1) {
        warn.log() << "file_ext: ";
        for (size_t i = 0; i < valueContent.size(); ++i) {
            warn.log() << valueContent[i] << " ";
        }
        warn.log() << ", fail to parse field, it accept only one value." << std::endl;
        throw ServerConfWarn();
    }
    if (!isValidFileExt(valueContent[0])) {
        warn.log() << "file_ext: " << valueContent[0] << " is not a valid file extension." << std::endl;
        throw ServerConfWarn();
    }
    _cgiPath = valueContent[0];
}
