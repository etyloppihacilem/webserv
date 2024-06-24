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

#include "Body.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include <map>
#include <string>

class ClientRequest {
    public:
        ClientRequest();
        ~ClientRequest();

        bool                                parse_header(const std::string &in);
        bool                                init_body(std::string &buffer, int fd);
        void                                save_mem();
    private:
        HttpMethod                          parse_method(const std::string &method, const size_t &end);
        void                                parse_target(const std::string &in, const size_t &pos);
        void                                decode_target();
        void                                parse_parameters();
        void                                init_header(const std::string &in);
        void                                parse_header_line(const std::string &in,
            size_t                                                              begin,
            size_t                                                              end);

        std::string                         _method;        ///< Method used for request
        std::string                         _target;        ///< Target of request
        std::map<std::string, std::string>  _header;        ///< Map containing headers
        std::map<std::string, std::string>  _parameters;    ///< Parameters from request
        bool                                _body_exists;   ///< True if there's a body in this request.
        /**< Determined by the presence of Content-Length or Transfer-Encoding headers in request.*/
        Body                                *_body;         ///< Pointer on body object if present
        HttpCode                            _status;        ///< Status of request (not 200 if error)
        bool                                _absolute_form; ///< True if request is in absolute form.
        /**< Absolute form means having the Host value in the request line and having no 'Host' header.
             Any 'Host' headers found while in absolute form is discarded.
        */

#ifdef TESTING
        FRIEND_TEST(ClientRequestTestSuite,     ParseMethodTestExpectedOK);
        FRIEND_TEST(ClientRequestTestSuite,     ParseMethodTestExpectedFail);
        FRIEND_TEST(ClientRequestTestTarget,    ParseTargetTest);
        FRIEND_TEST(ClientRequestTestSuite,     ParseHeaderLineTestHost);
        FRIEND_TEST(ClientRequestTestParseHeader, ParseHeaderLineTest);
        FRIEND_TEST(ClientRequestTestInitHeader, InitHeaderTest);
        FRIEND_TEST(ClientRequestTestSuite,     decode_target);
        FRIEND_TEST(ClientRequestTestSuite,     parse_parameters);
#endif
};

#endif  // INCLUDE_SRC_MESSAGE_HPP_
