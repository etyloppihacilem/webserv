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

    if (cleanStr == "/")
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

/**
  RFC 9112, $2.2:
  ===

  Although the line terminator for the start-line and fields is the sequence CRLF, a recipient MAY recognize a single LF
  as a line terminator and ignore any preceding CR.

  A sender MUST NOT generate a bare CR (a CR character not immediately followed by LF) within any protocol elements
  other than the content. A recipient of such a bare CR MUST consider that element to be invalid or replace each bare CR
  with SP before processing the element or forwarding the message.
  */
std::string &sanitize_HTTP_string(std::string &s) {
    size_t found = 0;
    while ((found = s.find("\r\n", found)) != s.npos)
        s.replace(found, 2, "\n");
    found = 0;
    while ((found = s.find("\r", found)) != s.npos)
        s.replace(found, 1, " ");
    return s;
}

// this exists mostly for testing purposes
std::string sanitize_HTTP_string(const std::string &s) {
    std::string tmp = s;
    return sanitize_HTTP_string(tmp);
}

// this exists mostly for testing purposes
std::string sanitize_HTTP_string(const char *s) {
    std::string tmp = std::string(s);
    return sanitize_HTTP_string(tmp);
}
