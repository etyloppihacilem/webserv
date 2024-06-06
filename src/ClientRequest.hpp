/* #############################################################################

               """          ClientRequest.hpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 17:51
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_MESSAGE_HPP_
#define INCLUDE_SRC_MESSAGE_HPP_

#ifdef TESTING
#include "gtest/gtest.h"
#endif

#include <map>
#include <sstream>
#include <string>
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Body.hpp"

class ClientRequest {
    public:
        ClientRequest();
        ClientRequest(const ClientRequest&);
        ~ClientRequest();

        ClientRequest                             &operator=(const ClientRequest&);
        bool                                parse_header(const std::string &in);
        bool                                init_body(std::string &buffer, int fd);
    private:
        HttpMethod                          parse_method(const std::string &method, const size_t &end);
        void                                parse_target(const std::string &in, const size_t &pos);
        void                                init_header(const std::string &in);
        void                                parse_header_line(const std::string &in,
            size_t                                                              begin,
            size_t                                                              end);

        std::string                         _method;
        std::string                         _target;
        std::map<std::string, std::string>  _header;
        std::map<std::string, std::string>  _parameters;
        bool                                _body_exists;
        Body                                *_body;
        HttpCode                            _status;
        bool                                _absolute_form;

#ifdef TESTING
        FRIEND_TEST(ClientRequestTestSuite,   ParseMethodTestExpectedOK);
        FRIEND_TEST(ClientRequestTestSuite,   ParseMethodTestExpectedFail);
        FRIEND_TEST(ClientRequestTestTarget,  ParseTargetTest);
        FRIEND_TEST(ClientRequestTestSuite,   ParseHeaderLineTestHost);
        FRIEND_TEST(ClientRequestTestParseHeader, ParseHeaderLineTest);
        FRIEND_TEST(ClientRequestTestInitHeader,  InitHeaderTest);
#endif
};

#endif  // INCLUDE_SRC_MESSAGE_HPP_
