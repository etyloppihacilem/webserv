#include "ServerConfValidate.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfTokenize.hpp"
#include "StringTokenizer.hpp"
#include "StringUtils.hpp"
#include <cctype>
#include <climits>
#include <cstdlib>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

// NOTE: might me useless
//  example of fileExt: html, conf, etc...
/*bool isMatchingFileExt(const std::string &configFile, const std::string &fileExt) {*/
/*    if (extract_extension(configFile) == fileExt)*/
/*        return true;*/
/*    return false;*/
/*}*/

// this is a simplified way of checking if the url is valid, can't use sys fct to check it
bool isValidHostname(const std::string &value) {
    if (value.empty() || *value.begin() == '-' || *value.rbegin() == '-')
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
    if (hasOneAplha)
        return true;
    return false;
}

// Split the first element at every '.'
// For an IP address you should have four elements.
// Check that all elements in both vectors are integers between 0 and 255.
bool isValidIPAddress(const std::string &value) {
    if (value.empty())
        return false;

    StringTokenizer IPString(value, '.');
    ValueList       IPoctets = getFieldValues(IPString);
    if (IPoctets.size() != 4)
        return false;
    for (int i = 0; i < 4; ++i) {
        int octet = atoi(IPoctets[i].c_str());

        if (octet < 0 || octet > 255)
            return false;
    }
    return true;
}

bool isValidPath(const std::string &value) {
    if (value.empty())
        return false;

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
    if (!value.empty() && value[0] != '/' && isValidPath(value))
        return true;
    return false;
}

bool isValidAbsolutePath(const std::string &value) {
    if (!value.empty() && value[0] == '/' && isValidPath(value))
        return true;
    return false;
}

bool isValidExtension(const std::string &value) {
    if (!value.empty() && value[0] == '.' && extract_extension(value) == "")
        return true;
    return false;
}
