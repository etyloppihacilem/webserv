/* #############################################################################

               """          Response.hpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:47
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_RESPONSE_HPP_
#define INCLUDE_SRC_RESPONSE_HPP_

#include "BodyWriter.hpp"
#include "HttpStatusCodes.hpp"
#include <map>
#include <string>

#ifdef TESTING
#include "gtest/gtest.h"
#endif

typedef std::map<std::string, std::string>::const_iterator mapit;

class Response {
    public:
        Response();
        ~Response();

        void                                set_body(BodyWriter *body);
        void                                set_body(const std::string &data);
        void                                add_header(const std::string &field, const std::string &value);
        void                                set_code(const HttpCode &code);
        HttpCode                            get_code() const;
        std::string                         build_response();

    private:
        std::string                         generate_status_line() const;
        std::string                         generate_header() const;

        HttpCode                            _code;
        std::map<std::string, std::string>  _header;
        BodyWriter                          *_body;
#ifdef TESTING
        FRIEND_TEST(ResponseTestSuite, generate_status_line);
#endif
};

#endif  // INCLUDE_SRC_RESPONSE_HPP_
