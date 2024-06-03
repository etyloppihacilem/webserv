/* #############################################################################

               """          BodyLength.cpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:52
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyLength.hpp"
#include "Body.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include <cctype>
#include <sstream>
#include <string>

BodyLength::BodyLength(int fd, std::string &buffer, std::string length):
    Body(fd, buffer) {
    for (std::string::iterator i = length.begin(); i != length.end(); i++)
    {
        if (!isdigit(*i))
            throw (HttpError(BadRequest)); // inbalid length
    }
    std::stringstream tmp(length);
    if (!tmp >> _length)
        throw (HttpError(BadRequest)); // other invalid length
}

BodyLength::~BodyLength() {}

std::string BodyLength::get() {
    // trouver quelle buffer size utiliser
    return ("");
}

std::string BodyLength::get(size_t size) {
    (void) size;
    return ("");
}
