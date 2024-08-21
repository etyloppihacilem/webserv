/* #############################################################################

               """          StringUtils.cpp
        -\-    _|__
         |\___/  . \        Created on 12 Jun. 2024 at 14:48
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "StringUtils.hpp"
#include <cstddef>
#include <string>

std::string add_trailing_slash(const std::string &str) {
    std::string retStr(str);

    if (!retStr.empty() && retStr.at(retStr.size() - 1) != '/')
        retStr.push_back('/');
    return retStr;
}

std::string clean_trailing_slash(const std::string &str) {
    std::string cleanStr(str);

    if (cleanStr == "/" )
        return cleanStr;
    if (!cleanStr.empty() && cleanStr.at(cleanStr.size() - 1) == '/')
        cleanStr.erase(cleanStr.size() - 1, 1);
    return cleanStr;
}

/**
  Shrink std::string allocation to match size.
  Saves memory.
  */
void shrink_to_fit(std::string &str) {
    std::string(str.data(), str.size()).swap(str);
}

/**
  Extract extension from path.
  */
std::string extract_extension(const std::string &s) {
    size_t slash     = s.find_last_of("/");
    size_t extension = s.find_last_of(".");

    if (slash == s.npos)
        slash = 0;
    if (extension == s.npos || (slash == 0 && extension == slash) || (slash != 0 && extension == slash + 1))
        return "";
    return s.substr(extension + 1, s.length() - (extension + 1));
}

std::string extract_basename(const std::string &s) {
    size_t slash = s.find_last_of("/");

    if (slash == s.npos)
        slash = 0;
    else
        slash += 1;
    return s.substr(slash, s.length() - slash);
}
