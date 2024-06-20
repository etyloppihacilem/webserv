#include "ServerConfTokenize.hpp"
#include "ServerConfValidate.hpp"
#include <algorithm>

std::string tokenizeFile(const std::string &input)
{
    std::string tokenString(input);
    const char  delim = '|';

    std::   replace(tokenString.begin(),    tokenString.end(),  ' ',    delim);
    std::   replace(tokenString.begin(),    tokenString.end(),  '\t',   delim);

    StringTokenizer tokenizedFile(tokenString, "|");
    std::string     currentToken = tokenizedFile.nextToken();   // remove http

    if (currentToken != ConfFieldString(http)) {
        std::replace(tokenizedFile.remainingString().begin(), tokenizedFile.remainingString().end(), "|", " ");
        throw ServerConfError(tokenizedFile.remainingString().substr(0, 30)
                + " ... : the first token is not 'http', parsing canceled");
    }
    currentToken = tokenizedFile.nextToken();                   // remove open bracket
    if (currentToken != "{") {
        std::replace(tokenizedFile.remainingString().begin(), tokenizedFile.remainingString().end(), "|", " ");
        throw ServerConfError(tokenizedFile.remainingString().substr(0, 30)
                + " ... : token '{' is missing after token 'http' , parsing canceled");
    }

    size_t closingBracePos = findClosingBrace(tokenizedFile.remainingString());

    if (closingBracePos == std::string::npos) {
        std::replace(tokenizedFile.remainingString().begin(), tokenizedFile.remainingString().end(), "|", " ");
        throw ServerConfError(tokenizedFile.remainingString().substr(0, 30)
                + " ... : token '}' is missing unable to locate the end of 'http' module , parsing canceled");
    }
    return tokenizedFile.nextToken(closingBracePos, "}");
}

std::string tokenizeServer(StringTokenizer &tokenizedFile)
{
    std::string currentToken = tokenizedFile.nextToken();   // remove Server

    if (currentToken != ConfFieldString(server)) {
        std::replace(tokenizedFile.remainingString().begin(), tokenizedFile.remainingString().end(), "|", " ");
        throw ServerConfError(tokenizedFile.remainingString().substr(0, 30)
                + " ... : the first token is not 'server', parsing canceled");
    }
    currentToken = tokenizedFile.nextToken();               // remove open bracket
    if (currentToken != "{") {
        std::replace(tokenizedFile.remainingString().begin(), tokenizedFile.remainingString().end(), "|", " ");
        throw ServerConfError(tokenizedFile.remainingString().substr(0, 30)
                + " ... : token '{' is missing after token 'server' , parsing canceled");
    }

    size_t closingBracePos = findClosingBrace(tokenizedFile.remainingString());

    if (closingBracePos == std::string::npos) {
        std::replace(tokenizedFile.remainingString().begin(), tokenizedFile.remainingString().end(), "|", " ");
        throw ServerConfError(tokenizedFile.remainingString().substr(0, 30)
                + " ... : token '}' is missing unable to locate the end of 'server' module , parsing canceled");
    }
    return tokenizedFile.nextToken(closingBracePos, "}");
}

Field tokenizeLocation(StringTokenizer &tokenizedServer)
{
    Field locationInfo;

    std::string currentToken = tokenizedServer.nextToken(); // remove location field name

    if (currentToken != ConfFieldString(location)) {
        std::replace(tokenizedServer.remainingString().begin(), tokenizedServer.remainingString().end(), "|", " ");
        throw ServerConfError(tokenizedServer.remainingString().substr(0, 30)
                + " ... : the first token is not 'location', parsing canceled");
    }
    locationInfo.first  = tokenizedServer.nextToken();      // extract location root
    currentToken        = tokenizedServer.nextToken();      // remove open bracket
    if (currentToken != "{") {
        std::replace(tokenizedServer.remainingString().begin(), tokenizedServer.remainingString().end(), "|", " ");
        throw ServerConfError(tokenizedServer.remainingString().substr(0, 30)
                + " ... : token '{' is missing after token 'location' , parsing canceled");
    }

    size_t closingBracePos = findClosingBrace(tokenizedServer.remainingString());

    if (closingBracePos == std::string::npos) {
        std::replace(tokenizedServer.remainingString().begin(), tokenizedServer.remainingString().end(), "|", " ");
        throw ServerConfError(tokenizedServer.remainingString().substr(0, 30)
                + " ... : token '}' is missing unable to locate the end of 'location' module , parsing canceled");
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
        throw ServerConfWarn(fieldInfo.first + ": field name is not listed in server conf.");
    }
    fieldInfo.second = tokenizedServer.nextToken(";");  // remove open bracket
    if (fieldInfo.second.empty()) {
        throw ServerConfWarn(fieldInfo.first + ": field value is empty.");
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
