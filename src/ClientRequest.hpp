/* #############################################################################

               """          ClientRequest.hpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 17:51
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_MESSAGE_HPP_
#define INCLUDE_SRC_MESSAGE_HPP_

#include <cstddef>
#ifdef TESTING
# include "gtest/gtest.h"
#endif

#include "Body.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include <map>
#include <string>

class ClientRequest {
    public:
        ClientRequest(int socket, const std::string &ip);
        ClientRequest(int socket, HttpCode code, int port);
        ~ClientRequest();

        bool parse_request_line(std::string &in);
        bool parse_headers(std::string &in);
        bool init_body(std::string &buffer);
        bool gateway_checks(int port); ///< Returns true on success
        // bool        parse(const std::string &in);

        std::string        get_target() const;
        HttpMethod         get_method();
        Body              *get_body();
        void               remove_body();
        bool               have_body() const;
        HttpCode           get_status() const;
        int                get_socket() const;
        int                get_port() const;
        const std::string &get_query_string() const;
        const std::string &get_ip() const;

        const std::map< std::string, std::string > &get_header();
        // std::map<std::string, std::string>  &get_parameters();

        void save_mem();

    private:
        HttpMethod parse_method(const std::string &method, size_t end);
        void       parse_target(const std::string &in, size_t pos);
        void       decode_target();
        void       parse_parameters();
        bool       init_header(std::string &in);
        void       parse_header_line(const std::string &in, size_t begin, size_t end);
        void       parse_port();

        int                                  _socket;
        HttpMethod                           _method;      ///< Method used for request
        std::string                          _target;      ///< Target of request
        std::map< std::string, std::string > _header;      ///< Map containing headers
        bool                                 _body_exists; ///< True if there's a body in this request.
        /**< Determined by the presence of Content-Length or Transfer-Encoding headers in request.*/
        Body                                *_body;          ///< Pointer on body object if present
        HttpCode                             _status;        ///< Status of request (not 200 if error)
        bool                                 _absolute_form; ///< True if request is in absolute form.
        int                                  _port;          ///< Port of the request.
        std::string                          _query_string;
        std::string                          _ip;
        /**< Absolute form means having the Host value in the request line and having no 'Host' header.
             Any 'Host' headers found while in absolute form is discarded.
        */
        // std::map<std::string, std::string>  _parameters;    ///< Parameters from request

#ifdef TESTING
        FRIEND_TEST(ClientRequestTestSuite, ParseMethodTestExpectedOK);
        FRIEND_TEST(ClientRequestTestSuite, ParseMethodTestExpectedFail);
        FRIEND_TEST(ClientRequestTestTarget, ParseTargetTest);
        FRIEND_TEST(ClientRequestTestSuite, ParseHeaderLineTestHost);
        FRIEND_TEST(ClientRequestTestParseHeader, ParseHeaderLineTest);
        FRIEND_TEST(ClientRequestTestInitHeader, InitHeaderTest);
        FRIEND_TEST(ClientRequestTestSuite, decode_target);
        FRIEND_TEST(ClientRequestTestSuite, decode_target_loop);
        FRIEND_TEST(ClientRequestTestSuite, parse_parameters);
        FRIEND_TEST(ClientRequestTestSuite, parse_port);
        FRIEND_TEST(ResponseBuildStateFixture, HaveCorrectRoot);
#endif
};

#endif // INCLUDE_SRC_MESSAGE_HPP_
