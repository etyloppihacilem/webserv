#include "StringTokenizer.hpp"
#include <cstddef>
#include <string>

StringTokenizer::StringTokenizer(const std::string &str, const std::string &delim) {
    if (str.empty() || delim.empty()) {
        _tokenString = "";
        return;
    }
    _tokenString    = str;
    _delimiter      = delim;
    _delimiterLen   = delim.size();

    /* Remove sequential delimiter */
    std::size_t currentPosition = 0;

    while (true) {
        currentPosition = _tokenString.find(_delimiter, currentPosition);
        if (currentPosition != std::string::npos) {
            currentPosition += _delimiterLen;
            while (_tokenString.find(_delimiter, currentPosition) == currentPosition) {
                _tokenString.erase(currentPosition, _delimiterLen);
            }
        } else {
            break;
        }
    }
    /* Trim leading delimiter */
    if (_tokenString.find(_delimiter, 0) == 0) {
        _tokenString.erase(0, _delimiterLen);
    }
    /* Trim ending delimiter */
    if ((currentPosition = _tokenString.rfind(_delimiter)) != std::string::npos) {
        if (currentPosition != (_tokenString.size() - _delimiterLen)) {
            return;
        }
        _tokenString.erase(_tokenString.size() - _delimiterLen, _delimiterLen);
    }
}

StringTokenizer::~StringTokenizer() {}

bool StringTokenizer::hasMoreTokens()
{
    return _tokenString.size() > 0;
}

std::string StringTokenizer::remainingString()
{
    return _tokenString;
}

std::string StringTokenizer::delimValue()
{
    return _delimiter;
}

std::size_t StringTokenizer::delimLen()
{
    return _delimiterLen;
}

std::size_t StringTokenizer::countTokens()
{
    if (_tokenString.size() > 0) {
        std::size_t count           = 0;
        std::size_t currentPosition = 0;

        while (true) {
            currentPosition = _tokenString.find(_delimiter, currentPosition);
            if (currentPosition != std::string::npos) {
                ++count;
                currentPosition += _delimiterLen;
            } else {
                break;
            }
        }
        return ++count;
    }
    return 0;
}

std::size_t StringTokenizer::countTokens(const std::string &separator)
{
    if (!_tokenString.empty()) {
        std::size_t count           = 0;
        std::size_t currentPosition = 0;

        while (true) {
            currentPosition = _tokenString.find(separator, currentPosition);
            if (currentPosition != std::string::npos) {
                ++count;
                currentPosition += separator.size();
            } else {
                break;
            }
        }
        return ++count;
    }
    return 0;
}

std::string StringTokenizer::peakToken()
{
    if (_tokenString.empty()) {
        return "";
    }

    std::string token               = "";
    std::size_t delimiterPosition   = _tokenString.find(_delimiter, 0);

    if (delimiterPosition != std::string::npos) {
        token = _tokenString.substr(0, delimiterPosition);
    } else {
        token = _tokenString;
    }
    return token;
}

std::string StringTokenizer::nextToken()
{
    if (_tokenString.empty()) {
        return "";
    }

    std::string token               = "";
    std::size_t delimiterPosition   = _tokenString.find(_delimiter,
            0);

    if (delimiterPosition != std::string::npos) {
        token           = _tokenString.substr(0, delimiterPosition);
        _tokenString    = _tokenString.substr(delimiterPosition + _delimiterLen,
                _tokenString.size() - delimiterPosition);
    } else {
        token           = _tokenString.substr(0, _tokenString.size());
        _tokenString    = "";
    }
    return token;
}

std::string StringTokenizer::nextToken(const std::string &separator)
{
    if (_tokenString.empty()) {
        return "";
    }

    std::string token               = "";
    std::size_t separatorPosition   = _tokenString.find(separator, 0);

    if (separatorPosition != std::string::npos) {
        if (_tokenString.find(_delimiter, separatorPosition + 1) == separatorPosition + 1) {
            separatorPosition += _delimiterLen;
        }
        token           = _tokenString.substr(0, separatorPosition);
        _tokenString    = _tokenString.substr(separatorPosition + separator.size(),
                _tokenString.size() - separatorPosition);
    } else {
        token           = _tokenString.substr(0, _tokenString.size());
        _tokenString    = "";
    }
    return token;
}

std::string StringTokenizer::nextToken(std::size_t separatorPosition, const std::string &separator)
{
    if (_tokenString.empty()) {
        return "";
    }

    std::string token = "";

    if (separatorPosition != std::string::npos) {
        if (_tokenString.find(_delimiter, separatorPosition + 1) == separatorPosition + 1) {
            separatorPosition += _delimiterLen;
        }
        token           = _tokenString.substr(0, separatorPosition);
        _tokenString    = _tokenString.substr(separatorPosition + separator.size(),
                _tokenString.size() - separatorPosition);
    } else {
        token           = _tokenString.substr(0, _tokenString.size());
        _tokenString    = "";
    }
    return token;
}

std::string StringTokenizer::filterNextToken(const std::string &filterString)
{
    std::string tmpString   = nextToken();
    std::size_t currentPos  = 0;

    while ((currentPos = tmpString.find(filterString, currentPos)) != std::string::npos) {
        tmpString.erase(currentPos, filterString.size());
    }
    return tmpString;
}
