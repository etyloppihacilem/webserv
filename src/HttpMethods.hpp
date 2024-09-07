/* #############################################################################

               """          HttpMethods.hpp
        -\-    _|__
         |\___/  . \        Created on 07 May. 2024 at 11:05
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_HTTPMETHODS_HPP_
#define INCLUDE_SRC_HTTPMETHODS_HPP_

#include <string>

#define MIN_METHOD_SIZE 2
#define MAX_METHOD_SIZE 8

enum HttpMethod {
    GET = 0,
    POST,
    DELETE,
    PUT, // just in case
    HEAD,
    last,
};

#define COUNT_METHOD 4

inline std::string method_string(const int &method) {
    switch (method) {
        case GET:
            return "GET";
        case POST:
            return "POST";
        case DELETE:
            return "DELETE";
        case PUT:
            return "PUT";
        case HEAD:
            return "HEAD";
        default:
            return std::string();
    }
}

#endif // INCLUDE_SRC_HTTPMETHODS_HPP_
