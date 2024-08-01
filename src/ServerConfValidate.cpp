#include "ServerConfValidate.hpp"
#include "HttpMethods.hpp"
#include "ServerConfFields.hpp"
#include "StringTokenizer.hpp"
#include <cctype>
#include <climits>
#include <cstdlib>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

bool isValidConfFile(const std::string &configFile) {
    if (configFile.size() == 0)
        return false;

    std::string CONF = ".conf";

    if (isalnum(configFile[0]) && configFile.find('.', 0) != std::string::npos
        && configFile.find(CONF, 0) == configFile.size() - CONF.size()) {
        return true;
    }
    return false;
}

bool isValidFieldName(const std::string &name) {
    for (int i = 0; i < COUNT_CONF_FIELD; i++)
        if (name == ConfFieldString(i))
            return true;
    return false;
}

bool isValidHttp(const std::string &value) // TO DELETE
{
    if (value.size() == 0)
        return false;
    if (*value.begin() == '{' && *value.rbegin() == '}' && value.find("|server|") != std::string::npos)
        return true;
    return false;
}

bool isValidServer(const std::string &value) // TO DELETE
{
    if (value.size() == 0)
        return false;
    if (*value.begin() == '{' && *value.rbegin() == '}' && value.find("|server_name|") != std::string::npos
        && value.find("|listen|") != std::string::npos && value.find("|root|") != std::string::npos
        && value.find("|index|") != std::string::npos && value.find("|methods|") != std::string::npos) {
        return true;
    }
    return false;
}

bool isValidLocation(const std::string &value) // TO DELETE
{
    if (value.size() == 0)
        return false;
    if (*value.begin() == '{' && *value.rbegin() == '}' && value.find("|index|") != std::string::npos
        && value.find("|autoindex|") != std::string::npos) {
        return true;
    }
    return false;
}

bool isValidHostname(const std::string &value) {
    if (value.size() == 0 || *value.begin() == '-' || *value.rbegin() == '-')
        return false;

    bool hasOneAplha = false;

    for (std::string::const_iterator it = value.begin(); it < value.end(); ++it) {
        if (hasOneAplha == false && isalpha(*it))
            hasOneAplha = true;
        if (isalnum(*it)) {
            continue;
        } else if (*it == '.' || *it == '-') {
            if (*(it + 1) == *it)
                return false;
            continue;
        } else {
            return false;
        }
    }
    if (hasOneAplha == false)
        return false;
    return true;
}

// Split the first element at every '.'
// For an IP address you should have four elements.
// Check that all elements in both vectors are integers between 0 and 255.

bool isValidIPAddress(const std::string &value) {
    if (value.size() == 0)
        return false;

    StringTokenizer          IPString(value, '.');
    std::vector<std::string> IPoctets;

    while (IPString.hasMoreTokens())
        IPoctets.push_back(IPString.nextToken());
    if (IPoctets.size() != 4)
        return false;
    for (int i = 0; i < 4; ++i) {
        int octet = atoi(IPoctets[i].c_str());

        if (octet < 0 || octet > 255)
            return false;
    }
    return true;
}

// Check that value match a 4 digits string.
// Accept ony port between 8000 and 8999.

bool isValidPort(int value) {
    if (value > 1024 && value <= MAX_PORT)
        return true;
    return false;
}

bool isValidMethods(const std::string &value) {
    size_t len = value.size();

    if (len < MIN_METHOD_SIZE || len > MAX_METHOD_SIZE)
        return false;
    if (value[0] == 'G' && value.find(method_string(GET)) == 0 && len == 3)
        return true;
    if (value[0] == 'P' && value.find(method_string(POST)) == 0 && len == 4)
        return true;
    if (value[0] == 'D' && value.find(method_string(DELETE)) == 0 && len == 6)
        return true;
    return false;
}

bool isValidPath(const std::string &value) {
    for (std::string::const_iterator it = value.begin(); it < value.end(); ++it)
        if (isalnum(*it))
            continue;
        else if (*it == '/' || *it == '-' || *it == '_' || *it == '.')
            continue;
        else
            return false;
    return true;
}

bool isValidRelativePath(const std::string &value) {
    if (value.size() == 0 || value[0] == '/' || !isValidPath(value))
        return false;
    return true;
}

bool isValidAbsolutePath(const std::string &value) {
    if (value.size() == 0 || value[0] != '/' || !isValidPath(value))
        return false;
    return true;
}

bool isValidUrl(const std::string &value) {
    if (value.size() == 0 || value[0] != '/' || !isValidPath(value))
        return false;
    return true;
}

bool isValidIndexFile(const std::string &value) {
    if (value.size() == 0)
        return false;

    std::string HTML = ".html";

    if (isalnum(value[0]) && value.find('.', 0) != std::string::npos
        && value.find(HTML, 0) == value.size() - HTML.size()) {
        return true;
    }
    return false;
}

bool isValidClientMaxBodySize(const std::string &value) {
    long maxBodySize = atol(value.c_str());

    if (maxBodySize > 0 && maxBodySize <= INT_MAX)
        return true;
    return false;
}

bool isValidAutoindex(const std::string &value) {
    size_t len = value.size();

    if (len < 4 || len > 5)
        return false;
    if (value[0] == 'f' && value.find("false") == 0 && len == 5)
        return true;
    if (value[0] == 't' && value.find("true") == 0 && len == 4)
        return true;
    return false;
}

bool isValidFileExt(const std::string &value) {
    if (value.size() == 0)
        return false;
    if (value[0] == '.')
        return true;
    return false;
}
