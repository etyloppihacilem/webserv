#include "Logger.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfTokenize.hpp"
#include "ServerConfValidate.hpp"
#include "StringTokenizer.hpp"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <ostream>
#include <string>

std::string tokenizeFile(const std::string &input)
{
    std::string tokenString(input);
    const char  delim = '|';

    std::   replace(tokenString.begin(),    tokenString.end(),  ' ',    delim);
    std::   replace(tokenString.begin(),    tokenString.end(),  '\t',   delim);

    StringTokenizer tokenizedFile(tokenString, "|");
    std::string     errorString = tokenizedFile.remainingString().substr(0, 30);

    std::   replace(errorString.begin(),    errorString.end(),  '|',    ' ');

    std::string currentToken = tokenizedFile.nextToken();       // remove http

    if (currentToken != ConfFieldString(http)) {
        error.log() << errorString << " ... : the first token is not 'http', parsing canceled" << std::endl;
        throw ServerConfError();
    }
    currentToken = tokenizedFile.nextToken();                   // remove open bracket
    if (currentToken != "{") {
        error.log() << errorString << " ... : token '{' is missing after token 'http' , parsing canceled" << std::endl;
        throw ServerConfError();
    }

    size_t closingBracePos = findClosingBrace(tokenizedFile.remainingString());

    if (closingBracePos == std::string::npos) {
        error.log() << errorString
                    << " ... : token '}' is missing unable to locate the end of 'http' module , parsing canceled"
                    << std::endl;
        throw ServerConfError();
    }
    return tokenizedFile.nextToken(closingBracePos, "}");
}

std::string tokenizeServer(StringTokenizer &tokenizedFile)
{
    std::string errorString = tokenizedFile.remainingString().substr(0, 30);

    std::replace(errorString.begin(), errorString.end(), '|', ' ');

    std::string currentToken = tokenizedFile.nextToken();   // remove Server

    if (currentToken != ConfFieldString(server)) {
        error.log() << errorString << " ... : the first token is not 'server', parsing canceled" << std::endl;
        throw ServerConfError();
    }
    currentToken = tokenizedFile.nextToken();               // remove open bracket
    if (currentToken != "{") {
        error.log() << errorString << " ... : token '{' is missing after token 'server' , parsing canceled"
                    << std::endl;
        throw ServerConfError();
    }

    size_t closingBracePos = findClosingBrace(tokenizedFile.remainingString());

    if (closingBracePos == std::string::npos) {
        error.log() << errorString
                    << " ... : token '}' is missing unable to locate the end of 'server' module , parsing canceled"
                    << std::endl;
        throw ServerConfError();
    }
    return tokenizedFile.nextToken(closingBracePos, "}");
}

Field tokenizeLocation(StringTokenizer &tokenizedServer)
{
    Field       locationInfo;
    std::string errorString = tokenizedServer.remainingString();

    std::replace(errorString.begin(), errorString.end(), '|', ' ');

    std::string currentToken = tokenizedServer.nextToken(); // remove location field name

    if (currentToken != ConfFieldString(location)) {
        error.log() << errorString << " ... : the first token is not 'location', parsing canceled" << std::endl;
        throw ServerConfError();
    }
    locationInfo.first  = tokenizedServer.nextToken();      // extract location root
    currentToken        = tokenizedServer.nextToken();      // remove open bracket
    if (currentToken != "{") {
        error.log() << errorString << " ... : token '{' is missing after token 'location' , parsing canceled"
                    << std::endl;
        throw ServerConfError();
    }

    size_t closingBracePos = findClosingBrace(tokenizedServer.remainingString());

    if (closingBracePos == std::string::npos) {
        error.log() << errorString
                    << " ... : token '}' is missing unable to locate the end of 'location' module , parsing canceled"
                    << std::endl;
        throw ServerConfError();
    }
    locationInfo.second = tokenizedServer.nextToken(closingBracePos, "}");
    return locationInfo;
}

Field tokenizeField(StringTokenizer &tokenizedServer)
{
    Field fieldInfo;

    fieldInfo.first = tokenizedServer.nextToken();      // remove Server
    if (isValidFieldName(fieldInfo.first)) {
        tokenizedServer.nextToken(";");
        warn.log() << fieldInfo.first << ": field name is not listed in server conf." << std::endl;
        throw ServerConfWarn();
    }
    fieldInfo.second = tokenizedServer.nextToken(";");  // remove open bracket
    if (fieldInfo.second.empty()) {
        warn.log() << fieldInfo.first << ": field value is empty." << std::endl;
        throw ServerConfWarn();
    }
    return fieldInfo;
}

ValueList tokenizeValue(const std::string &value)
{
    ValueList       valueList;
    StringTokenizer tokenizedValue(value, "|");

    while (tokenizedValue.hasMoreTokens()) {
        valueList.push_back(tokenizedValue.nextToken());
    }
    return valueList;
}

size_t findClosingBrace(const std::string &tokenString)
{
    size_t  ob_count;
    size_t  cb_count;

    if (tokenString.size() != 0)
        return 0;
    ob_count    = 1;
    cb_count    = 0;
    for (std::string::const_iterator it = tokenString.begin(); it < tokenString.end(); ++it) {
        if (*it == '{')
            ob_count++;
        if (*it == '}')
            cb_count++;
        if (ob_count == cb_count)
            return std::distance(tokenString.begin(), it);
    }
    return 0;
}

int findFieldCode(const std::string &fieldName)
{
    for (int i = 0; i < COUNT_CONF_FIELD; ++i) {
        if (fieldName == ConfFieldString(i)) {
            return i;
        }
    }
    return -1;
}
