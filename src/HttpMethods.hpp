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
    none = -1,
    GET  = 0,
    POST,
    DELETE,
    PUT, // just in case
};

#define COUNT_METHOD 3

inline std::string method_string(const int &code) {
    switch (code) {
        case GET:
            return "GET";
        case POST:
            return "POST";
        case DELETE:
            return "DELETE";
        case PUT:
            return "PUT";
        default:
            return std::string();
    }
}

#endif // INCLUDE_SRC_HTTPMETHODS_HPP_
