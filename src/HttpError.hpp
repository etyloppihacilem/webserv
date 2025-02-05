/* #############################################################################

               """          HttpError.hpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 16:03
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_HTTPERROR_HPP_
#define INCLUDE_SRC_HTTPERROR_HPP_

#include "HttpStatusCodes.hpp"
#include <exception>
#include <string>

class HttpError : public std::exception {
    public:
        HttpError(const HttpCode code = InternalServerError, std::string message = "") throw() :
            _code(code),
            _message(message != "" ? message : status_string(code)) {}

        virtual ~HttpError() throw() {}

        const char *what() const throw() { return _message.c_str(); }

        HttpCode get_code() const { return _code; }

        std::string get_message() const { return _message; }

    private:
        HttpCode    _code;
        std::string _message;
};

#endif // INCLUDE_SRC_HTTPERROR_HPP_
