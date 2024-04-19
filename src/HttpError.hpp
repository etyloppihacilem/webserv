/* #############################################################################

               """          HttpError.hpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 16:03
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#pragma once
#ifndef INCLUDE_SRC_HTTPERROR_HPP_
#define INCLUDE_SRC_HTTPERROR_HPP_

#include <exception>
#include <string>

class HttpError: public std::exception {
    public:
        HttpError(const int code) throw ();

        virtual ~HttpError() throw () {}

        const char  *what() const throw () {
            return (_message.c_str());
        }

    // Method to generate a request

    private:
        int         _code;
        std::string _message;
};

#endif  // INCLUDE_SRC_HTTPERROR_HPP_
