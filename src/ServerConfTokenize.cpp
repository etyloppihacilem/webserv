#include "ServerConfTokenize.hpp"
#include "Logger.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfLogging.hpp"
#include "StringTokenizer.hpp"
#include <cstddef>
#include <iterator>
#include <ostream>
#include <string>

// NOTE: this could actually be done before parsing while reading in order to prompt the line number if possible
// this would remove all ServerConfError in ServerConfTokenizebut

size_t findClosingBrace(const std::string &tokenString) {
    size_t ob_count;
    size_t cb_count;

    if (tokenString.size() == 0)
        return 0;
    ob_count = 1;
    cb_count = 0;
    for (std::string::const_iterator it = tokenString.begin(); it < tokenString.end(); ++it) {
        if (*it == '{')
            ob_count++;
        if (*it == '}')
            cb_count++;
        if (ob_count == cb_count)
            return std::distance(tokenString.begin(), it);
    }
    return std::string::npos;
}

StringTokenizer tokenizeFile(const std::string &fileContent) {
    StringTokenizer tokenizedFile(fileContent, '|');
    std::string     loggingInfo = buildLoggingInfo(tokenizedFile.remainingString());

    std::string currentToken = tokenizedFile.nextToken();
    if (currentToken != ConfFieldString(http)) {
        logMissingFirstToken(http, loggingInfo);
        throw ServerConfError();
    }

    currentToken = tokenizedFile.nextToken();
    if (currentToken != "{") {
        logMissingOpenBrace(http, loggingInfo);
        throw ServerConfError();
    }

    size_t closingBracePos = findClosingBrace(tokenizedFile.remainingString());
    if (closingBracePos == std::string::npos
        || tokenizedFile.remainingString()[closingBracePos - 1] != tokenizedFile.delimiter()) {
        logMissingCloseBrace(http, loggingInfo);
        throw ServerConfError();
    }

    StringTokenizer tokenizedHttp(tokenizedFile.nextToken(closingBracePos), '|');
    if (tokenizedFile.hasMoreTokens()) {
        loggingInfo = buildLoggingInfo(tokenizedFile.remainingString());
        info.log() << loggingInfo << "... : config file has some unwanted trailing content after http scope."
                   << std::endl;
    }
    return tokenizedHttp;
}

StringTokenizer tokenizeServer(StringTokenizer &tokenizedHttp) {
    std::string loggingInfo = buildLoggingInfo(tokenizedHttp.remainingString());

    std::string currentToken = tokenizedHttp.nextToken();
    if (currentToken != ConfFieldString(server)) {
        logMissingFirstToken(server, loggingInfo);
        throw ServerConfError();
    }

    currentToken = tokenizedHttp.nextToken();
    if (currentToken != "{") {
        logMissingOpenBrace(server, loggingInfo);
        throw ServerConfError();
    }

    size_t closingBracePos = findClosingBrace(tokenizedHttp.remainingString());
    if (closingBracePos == std::string::npos
        || tokenizedHttp.remainingString()[closingBracePos - 1] != tokenizedHttp.delimiter()) {
        logMissingCloseBrace(server, loggingInfo);
        throw ServerConfError();
    }

    return StringTokenizer(tokenizedHttp.nextToken(closingBracePos), '|');
}

Field tokenizeLocation(StringTokenizer &tokenizedServer) {
    std::string loggingInfo = buildLoggingInfo(tokenizedServer.remainingString());
    Field       locationInfo;

    std::string currentToken = tokenizedServer.nextToken();
    if (currentToken != ConfFieldString(location)) {
        logMissingFirstToken(location, loggingInfo);
        throw ServerConfError();
    }

    locationInfo.first = tokenizedServer.nextToken();

    currentToken = tokenizedServer.nextToken();
    if (currentToken != "{") {
        logMissingOpenBrace(location, loggingInfo);
        throw ServerConfError();
    }

    size_t closingBracePos = findClosingBrace(tokenizedServer.remainingString());
    if (closingBracePos == std::string::npos
        || tokenizedServer.remainingString()[closingBracePos - 1] != tokenizedServer.delimiter()) {
        logMissingCloseBrace(location, loggingInfo);
        throw ServerConfError();
    }
    locationInfo.second = StringTokenizer(tokenizedServer.nextToken(closingBracePos), '|');

    return locationInfo;
}

Field tokenizeField(StringTokenizer &tokenizedServer) {
    Field fieldInfo;

    fieldInfo.first = tokenizedServer.nextToken();

    fieldInfo.second = StringTokenizer(tokenizedServer.nextToken(';'), '|');
    if (fieldInfo.second.remainingString().empty()) {
        warn.log() << fieldInfo.first << ": field value is empty." << std::endl;
        throw ServerConfWarn();
    }

    return fieldInfo;
}

ValueList getFieldValues(StringTokenizer &tokenizedValues) {
    ValueList valueList;

    while (tokenizedValues.hasMoreTokens())
        valueList.push_back(tokenizedValues.nextToken());
    return valueList;
}

ConfField getFieldCode(const std::string &fieldName) {
    for (int i = 0; i < COUNT_CONF_FIELD; ++i)
        if (fieldName == ConfFieldString(i))
            return static_cast< ConfField >(i);
    return static_cast< ConfField >(-1);
}
