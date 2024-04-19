/* #############################################################################

               """          HttpError.cpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 16:30
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "HttpError.hpp"

HttpError::HttpError(const int code) throw ():
    _code   (code),
    _message(status_string(code)) {}
