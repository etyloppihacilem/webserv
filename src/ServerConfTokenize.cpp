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
        return std::string();
    }
    currentToken = tokenizedFile.nextToken();                   // remove open bracket
    if (currentToken != "{") {
        return std::string();
    }

    size_t closingBracePos = findClosingBrace(tokenizedFile.remainingString());

    return tokenizedFile.nextToken(closingBracePos, "}");
}

std::string tokenizeServer(StringTokenizer &tokenizedFile)
{
    std::string currentToken = tokenizedFile.nextToken();   // remove Server

    if (currentToken != ConfFieldString(server)) {
        return std::string();
    }
    currentToken = tokenizedFile.nextToken();               // remove open bracket
    if (currentToken != "{") {
        return std::string();
    }

    size_t closingBracePos = findClosingBrace(tokenizedFile.remainingString());

    return tokenizedFile.nextToken(closingBracePos, "}");
}

Field tokenizeLocation(StringTokenizer &tokenizedServer)
{
    Field locationInfo;

    if (!isValidLocation(tokenizedServer.remainingString())) {
        throw ConfError();
    }

    std::string currentToken = tokenizedServer.nextToken(); // remove location

    if (currentToken != ConfFieldString(location)) {
        throw ConfError();
    }
    locationInfo.first  = tokenizedServer.nextToken();      // extract location root
    currentToken        = tokenizedServer.nextToken();      // remove open bracket
    if (currentToken != "{") {
        throw ConfError();
    }
    locationInfo.second = tokenizedServer.nextToken(findClosingBrace(tokenizedServer.remainingString()), "}");
    return locationInfo;
}

Field tokenizeField(StringTokenizer &tokenizedServer)
{
    Field       fieldInfo;
    std::string currentToken = tokenizedServer.nextToken(); // remove Server

    if (isValidFieldName(currentToken)) {
        tokenizedServer.nextToken(";");
        throw ConfError();
    }
    fieldInfo.first     = currentToken;
    fieldInfo.second    = tokenizedServer.nextToken(";"); // remove open bracket
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
