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
enum HttpMethods {
    none   = -1,
    GET    = 0,
    POST   = 1,
    DELETE = 2,
};

inline std::string method_string(const int &code) {
    switch (code) {
        case 0:
            return ("GET");
        case 1:
            return ("POST");
        case 2:
            return ("DELETE");
        default:
            return (std::string());
    }
}

#endif  // INCLUDE_SRC_HTTPMETHODS_HPP_
