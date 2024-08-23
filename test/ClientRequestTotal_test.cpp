/* #####################################################################################################################

               """          ClientRequestTotal_test.cpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 17:07
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ClientRequestTotal_test.hpp"
#include "ClientRequest.hpp"
#include "HttpError.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "HttpUtils.hpp"
#include "Logger.hpp"
#include "ReadState.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cassert>
#include <cstddef>
#include <map>
#include <string>
#include <unistd.h>
#include <vector>

std::vector< TotalRequest > TotalRequestData = {
    {
        "Basic_GET",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "/helloworld.html",
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
    {
        "Port_parsing",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1:42\r\nName: fireTesting/1.0\r\n\r\n",
        "/helloworld.html",
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        42,
        "hihi=ahah",
        false,
    },
    {
        "Wrong_port_parsing",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1:ab\r\nName: fireTesting/1.0\r\n\r\n",
        "/helloworld.html",
        GET,
        false,
        "",
        {},
        BadRequest,
        0,
        "hihi=ahah",
        false,
    },
    {
        "No_Host",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\r\nName: fireTesting/1.0\r\n\r\n",
        "/helloworld.html",
        GET,
        false,
        "",
        {},
        BadRequest,
        80,
        "hihi=ahah",
        false,
    },
    {
        "No_Headers",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\r\n\r\n",
        "",
        none,
        false,
        "",
        {},
        BadRequest,
        80,
        "",
        false,
    },
    {
        "No_Headers2",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\r\n\r\n\r\n",
        "",
        none,
        false,
        "",
        {},
        BadRequest,
        80,
        "",
        false,
    },
    {
        "Wrong_Method",
        "PET /helloworld.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "",
        none,
        false,
        "",
        {},
        NotImplemented,
        80,
        "",
        false,
    },
    {
        "PostLength",
        "POST /process.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n"
        "Content-Length: 97\r\n\r\nCoucou je suis heureux et c'est le premier body que nous allons pouvoir trouver"
        " dans ces tests...NOTBODY",
        "/process.html",
        POST,
        true,
        "Coucou je suis heureux et c'est le premier body que nous allons pouvoir trouver"
        " dans ces tests...",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
            { "Content-Length", "97" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
    {
        "PostChunk",
        "POST /process.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n"
        "Transfer-Encoding: chunk\r\n\r\na\r\nCoucou je \r\n32\r\nsuis heureux et c'est le premier body que nous "
        "all\r\n13\r\nons pouvoir trouver"
        "\r\n12\r\n dans ces tests...\r\n0\r\n\r\nNOTBODY",
        "/process.html",
        POST,
        true,
        "Coucou je suis heureux et c'est le premier body que nous allons pouvoir trouver"
        " dans ces tests...",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
            { "Transfer-Encoding", "chunk" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
    {
        "PostChunkTrailing",
        "POST /process.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n"
        "Transfer-Encoding: chunk\r\n\r\na\r\nCoucou je \r\n32\r\nsuis heureux et c'est le premier body que nous "
        "all\r\n13\r\nons pouvoir trouver"
        "\r\n12\r\n dans ces tests...\r\n0\r\nTrailingSecretData\r\nNOTBODY",
        "/process.html",
        POST,
        true,
        "Coucou je suis heureux et c'est le premier body que nous allons pouvoir trouver"
        " dans ces tests...",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
            { "Transfer-Encoding", "chunk" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
    {
        "PostChunkBad",
        "POST /process.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n"
        "Transfer-Encoding: chunk\r\n\r\na\r\nCoucou je \r\n30\r\nsuis heureux et c'est le premier body que nous "
        "all\r\n13\r\nons pouvoir trouver"
        "\r\n12\r\n dans ces tests...\r\n0\r\nTrailingSecretData\r\nNOTBODY",
        "/process.html",
        POST,
        true,
        "Coucou je suis heureux et c'est le premier body que nous allons pouvoir trouver"
        " dans ces tests...",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
            { "Transfer-Encoding", "chunk" },
        },
        OK,
        80,
        "hihi=ahah",
        true,
    },
    {
        "Redirect",
        "GET /helloworld.html?hihi=ahah&super=je suis heureux HTTP/1.1\r\nHost: 127.0.0.1\r\nName: "
        "fireTesting/1.0\r\n\r\n",
        "",
        GET,
        false,
        "",
        {
            { "Location", "/helloworld.html?hihi=ahah&super=je%20suis%20heureux" },
        },
        MovedPermanently,
        80,
        "",
        false,
    },
    {
        "Post_no_body",
        "POST /helloworld.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "/helloworld.html",
        POST,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
    {
        "Invalid_Header",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\r\nHost\r\nName: fireTesting/1.0\r\n\r\n",
        "",
        GET,
        false,
        "",
        {},
        BadRequest,
        80,
        "hihi=ahah",
        false,
    },
    {
        "Invalid_RequestLine",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\r\r\r\nName: fireTesting/1.0\r\n\r\n",
        "",
        GET,
        false,
        "",
        {},
        BadRequest,
        80,
        "",
        false,
    },
    {
        "InvalidHTTPVersion",
        "GET /helloworld.html?hihi=ahah HTTP/1.2\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "",
        GET,
        false,
        "",
        {},
        HTTPVersionNotSupported,
        80,
        "",
        false,
    },
    {
        "InvalidHTTPVersion2",
        "GET /helloworld.html?hihi=ahah HTTP/3.0\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "",
        GET,
        false,
        "",
        {},
        HTTPVersionNotSupported,
        80,
        "",
        false,
    },
    {
        "Percent_encoding",
        "GET /hello%20world.html?hihi=ahah&phrase=coucou%20je%20suis%20heureux%21 HTTP/1.1\r\nHost: 127.0.0.1\r\nName: "
        "fireTesting/1.0\r\n\r\n",
        "/hello world.html",
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "hihi=ahah&phrase=coucou%20je%20suis%20heureux%21",
        false,
    },
    {
        "Percent_encodinginjection",
        "GET /hello%20world.html%3f%3F?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "/hello world.html??",
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
    {
        "NoTarget",
        "GET ?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "",
        GET,
        false,
        "",
        {},
        BadRequest,
        0,
        "",
        false,
    },
    {
        "Absolute_Form",
        "GET http://coucou.je/suis/heureux?super=hihi HTTP/1.1\r\nName: fireTesting/1.0\r\n\r\n",
        "/suis/heureux",
        GET,
        false,
        "",
        {
            { "Host", "coucou.je" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "super=hihi",
        false,
    },
    {
        "Absolute_Form_Port",
        "GET http://coucou.je:543/suis/heureux?super=hihi HTTP/1.1\r\nName: fireTesting/1.0\r\n\r\n",
        "/suis/heureux",
        GET,
        false,
        "",
        {
            { "Host", "coucou.je" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        543,
        "super=hihi",
        false,
    },
    {
        "Absolute_Form_Override",
        "GET http://coucou.je/suis/heureux?super=hihi HTTP/1.1\r\nHost: super.fr\r\nName: fireTesting/1.0\r\n\r\n",
        "/suis/heureux",
        GET,
        false,
        "",
        {
            { "Host", "coucou.je" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "super=hihi",
        false,
    },
    {
        "Multiple_headers",
        "GET /coucou HTTP/1.1\r\ntest: hihi\r\nHost: super.fr\r\ntest: eheh\r\n\r\n",
        "/coucou",
        GET,
        false,
        "",
        {
            { "Host", "super.fr" },
            { "test", "hihi, eheh" },
        },
        OK,
        80,
        "",
        false,
    },
    {
        "wrong_headers",
        "GET /coucou HTTP/1.1\r\ntest: hihi\r\nHost: super.fr\r\ntest eheh\r\n\r\n",
        "",
        GET,
        false,
        "",
        {},
        BadRequest,
        80,
        "",
        false,
    },
    {
        "Bad_line_terminator_n",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\nHost: 127.0.0.1\nName: fireTesting/1.0\n\n",
        "/helloworld.html",
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
    {
        "Bad_line_terminator_r",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\rHost: 127.0.0.1\rName: fireTesting/1.0\r\r",
        "/helloworld.html",
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        BadRequest,
        80,
        "hihi=ahah",
        false,
    },
    {
        "Bad_line_terminator_r_as_space",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\r\nHost: \r127.0.0.1\r\nName:\r fireTesting/1.0\r\n\r\n",
        "/helloworld.html",
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
    {
        "Bad_header_after_name",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName : fireTesting/1.0\r\n\r\n",
        "/helloworld.html",
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
        },
        BadRequest,
        80,
        "hihi=ahah",
        false,
    },
    {
        "Bad_header_before_name",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\r\n Host: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "/helloworld.html",
        GET,
        false,
        "",
        {},
        BadRequest,
        80,
        "hihi=ahah",
        false,
    },
    {
        "leading_CRLF",
        "\r\nGET /helloworld.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "/helloworld.html",
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
    {
        "leading_CRLF2",
        "\r\n\r\nGET /helloworld.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "/helloworld.html",
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
    {
        "Request_line_limit",
        "GET " REQUEST_LINE_LIMIT_STRING " HTTP/1.1\r\nHost: 127.0.0.1\r\nName: "
        "fireTesting/1.0\r\n\r\n",
        REQUEST_LINE_LIMIT_STRING,
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
    {
        "Request_line_too_long",
        "GET  HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "/helloworld.html",
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
    {
        "Header_too_long",
        "GET /helloworld.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "/helloworld.html",
        GET,
        false,
        "",
        {
            { "Host", "127.0.0.1" },
            { "Name", "fireTesting/1.0" },
        },
        OK,
        80,
        "hihi=ahah",
        false,
    },
};

INSTANTIATE_TEST_SUITE_P(
    TotalRequestSuite,
    TotalRequestFixture,
    ::testing::ValuesIn(TotalRequestData),
    [](const testing::TestParamInfo< TotalRequest > &info) {
        // Can use info.param here to generate the test suffix
        std::string name = std::get< tname >(info.param);
        return name;
    }
);

TEST_P(TotalRequestFixture, TargetTest) {
    const std::string &correct = std::get< ttarget >(GetParam());

    if (_request->get_status() != OK)
        return;
    EXPECT_EQ(_request->get_target(), correct);
}

TEST_P(TotalRequestFixture, MethodTest) {
    HttpMethod correct = std::get< tmethod >(GetParam());

    if (_request->get_status() != OK)
        return;
    EXPECT_EQ(_request->get_method(), correct);
}

TEST_P(TotalRequestFixture, BodyTest) {
    typedef std::map< std::string, std::string > map;

    const std::string &correct = std::get< tbody >(GetParam());

    if (_request->get_status() != OK)
        return;
    if (!std::get< thavebody >(GetParam())) {
        warn.disable();
        EXPECT_EQ(_request->get_body(), (void *) 0);
        warn.enable();
        return;
    }
    ASSERT_TRUE(_request->have_body());

    const map &headers = _request->get_header();

    if (headers.find("Content-Length") != headers.end())
        SUCCEED();
    else if (headers.find("Transfer-Encoding") != headers.end())
        SUCCEED();
    else
        FAIL() << "All body headers are missing.";

    auto   body = _request->get_body();
    size_t i    = 0;

    if (std::get< tbadbody >(GetParam())) {
        EXPECT_THROW(
            {
                while (!body->is_done() && i++ < MAX_REQUEST_LINE) {
                    body->read_body();
                    body->get();
                }
            },
            HttpError
        );
        if (i >= MAX_REQUEST_LINE)
            FAIL() << "Infinite loop in body getter, Body tests should fail too.";
        return;
    }
    while (!body->is_done() && i++ < MAX_REQUEST_LINE) {
        body->read_body();
        body->get();
    }
    if (i >= MAX_REQUEST_LINE)
        FAIL() << "Infinite loop in body getter, Body tests should fail too.";

    auto body_content = body->get();

    EXPECT_EQ(body_content, correct);
}

TEST_P(TotalRequestFixture, HeadersTest) {
    typedef std::map< std::string, std::string > map;

    const map &correct      = std::get< theaders >(GetParam());
    const map &test_headers = _request->get_header();

    if (isRedirection(std::get< tstatus >(GetParam()))) {
        ASSERT_NE(correct.find("Location"), correct.end())
            << "pas de location pour vérifier la redirection dans le test.";

        map::const_iterator loc = test_headers.find("Location");

        ASSERT_NE(loc, test_headers.end());
        EXPECT_EQ(correct.find("Location")->second, loc->second);
        return;
    }
    if (_request->get_status() != OK)
        return;

    map::const_iterator test_item;
    map::const_iterator correct_item;

    EXPECT_THAT(test_headers, ::testing::ContainerEq(correct));
}

TEST_P(TotalRequestFixture, HaveBodyTest) {
    const bool correct = std::get< thavebody >(GetParam());

    if (_request->get_status() != OK)
        return;
    EXPECT_EQ(correct, _request->have_body());
}

TEST_P(TotalRequestFixture, StatusTest) {
    const HttpCode correct = std::get< tstatus >(GetParam());

    EXPECT_EQ(correct, _request->get_status());
}

TEST_P(TotalRequestFixture, FdRequestTest) {
    const int correct = _fd_check;

    EXPECT_EQ(correct, _request->get_socket());
}

TEST_P(TotalRequestFixture, FdStateTest) {
    const int correct = _fd_check;

    EXPECT_EQ(correct, _test->_socket);
}

TEST_P(TotalRequestFixture, PortTest) {
    const int correct = std::get< tport >(GetParam());

    if (_request->get_status() != OK)
        return;
    EXPECT_EQ(correct, _request->get_port());
}

TEST_P(TotalRequestFixture, QueryStringTest) {
    const std::string &correct = std::get< tqs >(GetParam());

    if (_request->get_status() != OK)
        return;
    EXPECT_EQ(correct, _request->get_query_string());
}
