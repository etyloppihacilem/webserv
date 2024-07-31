#include "StringTokenizer.hpp"
#include "Logger.hpp"
#include "StringUtils.hpp"
#include <cstddef>
#include <ostream>
#include <string>

StringTokenizer::StringTokenizer(const std::string &str, const char delim): _tokenString(""), _delimiter(delim) {
    if (str.empty()) {
        _tokenString = "";
        return;
    }
    _tokenString = str;
    _delimiter   = delim;

    /* Remove sequential delimiter */
    std::size_t currentPosition = 0;

    while (true) {
        currentPosition = _tokenString.find(_delimiter, currentPosition);
        if (currentPosition != std::string::npos) {
            currentPosition += 1;
            while (_tokenString.find(_delimiter, currentPosition) == currentPosition)
                _tokenString.erase(currentPosition, 1);
        } else {
            break;
        }
    }
    /* Trim leading delimiter */
    if (_tokenString.find(_delimiter, 0) == 0)
        _tokenString.erase(0, 1);
    /* Trim ending delimiter */
    if ((currentPosition = _tokenString.rfind(_delimiter)) != std::string::npos) {
        if (currentPosition != (_tokenString.size() - 1))
            return;
        _tokenString.erase(_tokenString.size() - 1, 1);
    }
}

StringTokenizer::~StringTokenizer() {}

bool StringTokenizer::hasMoreTokens() {
    return _tokenString.size() > 0;
}

std::string StringTokenizer::remainingString() {
    return _tokenString;
}

char StringTokenizer::delimiter() {
    return _delimiter;
}

std::size_t StringTokenizer::countTokens() {
    if (_tokenString.size() > 0) {
        std::size_t count           = 0;
        std::size_t currentPosition = 0;

        while (true) {
            currentPosition = _tokenString.find(_delimiter, currentPosition);
            if (currentPosition != std::string::npos) {
                ++count;
                currentPosition += 1;
            } else {
                break;
            }
        }
        return ++count;
    }
    return 0;
}

std::size_t StringTokenizer::countTokens(const char separator) {
    if (!_tokenString.empty()) {
        std::size_t count           = 0;
        std::size_t currentPosition = 0;

        while (true) {
            currentPosition = _tokenString.find(separator, currentPosition);
            if (currentPosition != std::string::npos) {
                ++count;
                currentPosition += 1;
            } else {
                break;
            }
        }
        return ++count;
    }
    return 0;
}

std::string StringTokenizer::peakToken() {
    if (_tokenString.empty())
        return "";

    std::string token             = "";
    std::size_t delimiterPosition = _tokenString.find(_delimiter, 0);

    if (delimiterPosition != std::string::npos)
        token = _tokenString.substr(0, delimiterPosition);
    else
        token = _tokenString;
    return token;
}

std::string StringTokenizer::extractToken(std::size_t position) {

    std::string token = "";

    if (position != std::string::npos) {
        token        = _tokenString.substr(0, position);
        _tokenString = _tokenString.substr(position + 1, _tokenString.size() - position);
    } else {
        token        = _tokenString.substr(0, _tokenString.size());
        _tokenString = "";
    }
    shrink_to_fit(_tokenString);
    return token;
}

void StringTokenizer::removeTrailingDelimiter(std::string &token) {
    if (token.rfind(_delimiter) == token.size() - 1) {
        token = token.substr(0, token.size() - 1);
    }
}

std::string StringTokenizer::nextToken() {
    if (_tokenString.empty())
        return "";

    std::size_t delimiterPosition = _tokenString.find(_delimiter, 0);
    std::string token             = extractToken(delimiterPosition);

    return token;
}

std::string StringTokenizer::nextToken(const char separator) {
    if (_tokenString.empty())
        return "";

    std::size_t separatorPosition = _tokenString.find(separator, 0);
    std::string token             = extractToken(separatorPosition);

    removeTrailingDelimiter(token);
    return token;
}

std::string StringTokenizer::nextToken(std::size_t separatorPosition) {
    if (_tokenString.empty())
        return "";

    std::string token = extractToken(separatorPosition);

    removeTrailingDelimiter(token);
    return token;
}
