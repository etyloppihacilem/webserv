#include "Route.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfLogging.hpp"
#include "ServerConfSetter.hpp"
#include "ServerConfTokenize.hpp"
#include "StringTokenizer.hpp"
#include "StringUtils.hpp"
#include <ostream>
#include <set>
#include <string>
#include <vector>

// WARN: check with a mix between absolute and relative path !!!

Route::Setter Route::fieldSetterList[COUNT_CONF_FIELD] = {
    0,
    0,
    0,
    0,
    0,
    0,
    &Route::setRootDir,
    &Route::setIndexPage,
    &Route::setAutoindex,
    &Route::setMethods,
    0,
    &Route::setUpload,
    &Route::setRedirection,
    &Route::setCgiPath,
    &Route::setCgiExtension,
};

Route::Route() {}

Route::Route(Server &server) :
    _location("/"),
    _rootDir(server.getRootDir() + "/"),
    _indexPage(server.getIndexPage()),
    _autoindex(server.getAutoindex()),
    _methods(server.getMethods()),
    _uploadPath(_rootDir),
    _redirCode(OK),
    _redirPage(""),
    _cgiPath(""),
    _cgiExtension(""),
    _isFieldSet(COUNT_CONF_FIELD, false) {}

Route::Route(const std::string &locationPath, StringTokenizer &tokenizedLocation, Server &server) :
    _location(locationPath),
    _rootDir(server.getRootDir()),
    _indexPage(server.getIndexPage()),
    _autoindex(server.getAutoindex()),
    _methods(server.getMethods()),
    _uploadPath(server.getRootDir()),
    _redirCode(OK),
    _redirPage(""),
    _cgiPath(""),
    _cgiExtension(""),
    _isFieldSet(COUNT_CONF_FIELD, false) {

    logParsingStarted(location, tokenizedLocation.remainingString(), locationPath);

    while (tokenizedLocation.hasMoreTokens()) {
        try {
            Field     fieldContent = tokenizeField(tokenizedLocation);
            ConfField fieldCode    = getFieldCode(fieldContent.first);
            ValueList values       = getFieldValues(fieldContent.second);
            if (isRouteConfField(fieldCode) == wrongField) {
                warn.log() << fieldContent.first << ": is not a valide Route Conf Field" << std::endl;
                continue;
            }
            if (_isFieldSet[fieldCode]) {
                logFieldRedefinition(fieldCode, values);
                continue;
            }
            (this->*Route::fieldSetterList[fieldCode])(values);
            _isFieldSet[fieldCode] = true;
        } catch (ServerConfWarn &e) {
            continue;
        }
    }
    if (_isFieldSet[root] == false)
        _rootDir = add_trailing_slash(_rootDir += _location);
    if (_isFieldSet[upload_path] == false)
        _uploadPath = _rootDir;

    logParsingEnded(location);
}

Route::~Route() {}

bool Route::getAutoindex() const {
    return _autoindex;
}

std::string Route::getRootDir() const {
    return _rootDir;
}

std::set< HttpMethod > Route::getMethods() const {
    return _methods;
}

std::vector< std::string > Route::getIndexPage() const {
    return _indexPage;
}

std::string Route::getUploadPath() const {
    return _uploadPath;
}

HttpCode Route::getRedirCode() const {
    return _redirCode;
}

std::string Route::getRedirPage() const {
    return _redirPage;
}

std::string Route::getCgiPath() const {
    return _cgiPath;
}

std::string Route::getCgiExtension() const {
    return _cgiExtension;
}

std::string Route::getLocation() const {
    return _location;
}

bool Route::hasRootSet() const {
    return _isFieldSet[root];
}

bool Route::hasIndexPageSet() const {
    return _isFieldSet[index_f];
}

bool Route::hasAutoindexSet() const {
    return _isFieldSet[autoindex];
}

bool Route::hasMethodsSet() const {
    return _isFieldSet[methods];
}

bool Route::hasUploadSet() const {
    return _isFieldSet[upload_path];
}

bool Route::hasRedirSet() const {
    return _isFieldSet[rewrite];
}

bool Route::hasCgiPathSet() const {
    return _isFieldSet[cgi_path];
}

bool Route::hasCgiExtensionSet() const {
    return _isFieldSet[file_ext];
}

void Route::setRootDir(const ValueList &values) {
    _rootDir = setFieldRoot(values);
}

void Route::setIndexPage(const ValueList &values) {
    _indexPage = setFieldIndex(values);
}

void Route::setAutoindex(const ValueList &values) {
    _autoindex = setFieldAutoindex(values);
}

void Route::setMethods(const ValueList &values) {
    _methods = setFieldMethods(values);
}

void Route::setUpload(const ValueList &values) {
    _uploadPath = setFieldUploadPath(values);
}

void Route::setRedirection(const ValueList &values) {
    _redirCode = setFieldRewriteCode(values);
    _redirPage = values[1];
}

void Route::setCgiPath(const ValueList &values) {
    _cgiPath = setFieldCgiPath(values);
}

void Route::setCgiExtension(const ValueList &values) {
    _cgiExtension = setFieldFileExt(values);
}
