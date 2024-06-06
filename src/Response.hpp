/* #############################################################################

               """          Response.hpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:47
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_RESPONSE_HPP_
#define INCLUDE_SRC_RESPONSE_HPP_

#include "HttpStatusCodes.hpp"
#include <map>
#include <string>
class Response {
    public:
        Response();
        ~Response();

        void                                set_body(std::string body);
        void                                add_header(std::string field, std::string value);
        void                                set_code(HttpCode code);
        HttpCode                            get_code() const;
        std::string                         build_response();

    private:
        HttpCode                            _code;
        std::map<std::string, std::string>  _header;
        std::string                         _body;
};

#endif  // INCLUDE_SRC_RESPONSE_HPP_
