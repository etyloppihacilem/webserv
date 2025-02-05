/* #####################################################################################################################

               """          ResponseBuildState_test.cpp
        -\-    _|__
         |\___/  . \        Created on 28 Jul. 2024 at 16:08
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ResponseBuildState_test.hpp"
#include "BodyWriter.hpp"
#include "BodyWriterLength.hpp"
#include "CGIStrategy.hpp"
#include "DeleteStrategy.hpp"
#include "ErrorStrategy.hpp"
#include "FakeRoute.hpp"
#include "FakeServer.hpp"
#include "GetFileStrategy.hpp"
#include "GetIndexStrategy.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "RedirectStrategy.hpp"
#include "ResponseBuildState.hpp"
#include "UploadStrategy.hpp"
#include "todo.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cstddef>
#include <map>
#include <sstream>
#include <string>
#include <vector>

FakeServer ResponseBuildStateFixture::_server;

// Do not include default server header in header verification list.
// WARN: Do not test for CGI yet !! not implemented !!
std::vector< d_rbs > ResponseBuildData = {
    {
        "GET",
        "GET /index.html HTTP/1.1\r\nHost: coucou\r\n\r\n",
        tGetFileStrategy,
        OK,
        {
            { "Content-Type", "text/html" },
            { "Content-Length", "102" },
        },
        true,
        "<body><h1>Coucou je suis heureux</h1><div>This file is there for test and demo purposes.</div></body>\n",
        false,
        "/",
    },
    {
        "GET_not_found",
        "GET /indox.html HTTP/1.1\r\nHost: coucou\r\n\r\n",
        tErrorStrategy,
        NotFound,
        {
            { "Content-Type", "text/html; charset=utf-8" },
            { "Content-Length", "145" },
        },
        true,
        "<head><title>404 Error</title></head><body><h1>Error: 404 Not Found</h1><p>This error page was automatically "
        "generated.</p><p>" SERVER_SOFTWARE "</p></body>",
        false,
        "/",
    },
    {
        "POST_create",
        "POST /test/upload.txt HTTP/1.1\r\nHost: coucou\r\nContent-Length: 22\r\n\r\nCoucou je suis heureux",
        tUploadStrategy,
        Created,
        {
            { "Location", "/test/upload.txt" },
            { "Content-Length", "7" },
            { "Content-Type", "text/plain" },
        },
        true,
        "Success",
        true,
        "/test",
    },
    {
        "PUT_create",
        "PUT /test/upload.txt HTTP/1.1\r\nHost: coucou\r\nContent-Length: 22\r\n\r\nCoucou je suis heureux",
        tUploadStrategy,
        Created,
        {
            { "Location", "/test/upload.txt" },
            { "Content-Length", "7" },
            { "Content-Type", "text/plain" },
        },
        true,
        "Success",
        true,
        "/test",
    },
    {
        "DELETE",
        "DELETE /test/delete.png HTTP/1.1\r\nHost: coucou\r\nContent-Length: 22\r\n\r\nCoucou je suis heureux",
        tDeleteStrategy,
        NoContent,
        {},
        false,
        "",
        true,
        "/test",
    },
    {
        "DELETE_not_found",
        "DELETE /test/not_upload.txt HTTP/1.1\r\nHost: coucou\r\nContent-Length: 22\r\n\r\nCoucou je suis heureux",
        tErrorStrategy,
        NotFound,
        {
            { "Content-Type", "text/html; charset=utf-8" },
            { "Content-Length", "145" },
        },
        true,
        "<head><title>404 Error</title></head><body><h1>Error: 404 Not Found</h1><p>This error page was automatically "
        "generated.</p><p>" SERVER_SOFTWARE "</p></body>",
        false,
        "/test",
    },
    {
        "redirect",
        "GET /space_travel HTTP/1.1\r\nHost: coucou\r\n\r\n",
        tRedirectStrategy,
        MovedPermanently,
        {
            { "Location", "/star_wars/milky_way.html" },
        },
        false,
        "",
        false,
        "/space_travel",
    },
    {
        "redirect_discarding",
        "GET /space_travel/some/things HTTP/1.1\r\nHost: coucou\r\n\r\n",
        tRedirectStrategy,
        MovedPermanently,
        {
            { "Location", "/star_wars/milky_way.html" },
        },
        false,
        "",
        false,
        "/space_travel",
    },
    {
        "redirect_append_nothing",
        "GET /space_corridor HTTP/1.1\r\nHost: coucou\r\n\r\n",
        tRedirectStrategy,
        TemporaryRedirect,
        {
            { "Location", "http://space_corridor.fr/" },
        },
        false,
        "",
        false,
        "/space_corridor",
    },
    {
        "redirect_append",
        "GET /space_corridor/search/ici?ici=coucou HTTP/1.1\r\nHost: coucou\r\n\r\n",
        tRedirectStrategy,
        TemporaryRedirect,
        {
            { "Location", "http://space_corridor.fr/search/ici?ici=coucou" },
        },
        false,
        "",
        false,
        "/space_corridor",
    },
    {
        "indexStrategy",
        "GET /test/ HTTP/1.1\r\nHost: coucou\r\n\r\n",
        tGetIndexStrategy,
        OK,
        {
            { "Content-Type", "text/html; charset=utf-8" },
            { "Content-Length", "711" },
        },
        true,
        "<head></head><body><h1>/test/</h1><table><tr><td>Type</td><td>Name</td><td>size</td></tr><tr><td>DIR</"
        "td><td><a href=\"/test/.\">.</a></td><td>0</td></tr><tr><td>DIR</td><td><a "
        "href=\"/test/..\">..</a></td><td>0</td></tr><tr><td>REG</td><td><a "
        "href=\"/test/delete.png\">delete.png</a></td><td>0</td></tr><tr><td>REG</td><td><a "
        "href=\"/test/index.html\">index.html</a></td><td>11</td></tr><tr><td>REG</td><td><a "
        "href=\"/test/send.html\">send.html</a></td><td>406</td></tr><tr><td>REG</td><td><a "
        "href=\"/test/test.txt\">test.txt</a></td><td>33</td></tr><tr><td>DIR</td><td><a "
        "href=\"/test/upload\">upload</a></td><td>0</td></tr><tr><td>REG</td><td><a "
        "href=\"/test/upload_a.md\">upload_a.md</a></td><td>23</td></tr></table></body>",
        false,
        "/test",
    },
    {
        "indexStrategy_not_found",
        "GET /super/mais/non HTTP/1.1\r\nHost: coucou\r\n\r\n",
        tErrorStrategy,
        NotFound,
        {
            { "Content-Type", "text/html; charset=utf-8" },
            { "Content-Length", "145" },
        },
        true,
        "<head><title>404 Error</title></head><body><h1>Error: 404 Not Found</h1><p>This error page was automatically "
        "generated.</p><p>" SERVER_SOFTWARE "</p></body>",
        false,
        "/",
    },
    {
        "python_route_only",
        "GET /python/ HTTP/1.1\r\nHost: coucou\r\n\r\n",
        tGetFileStrategy,
        OK,
        {
            { "Content-Type", "text/html" },
            { "Content-Length", "6" },
        },
        true,
        "Hey:)\n",
        false,
        "/python",
    },
    {
        "upload_to_different_location",
        "POST /test/diff/hihi.txt HTTP/1.1\r\nHost: coucou\r\nContent-Length: 22\r\n\r\nCoucou je suis heureux",
        tUploadStrategy,
        Created,
        {
            { "Location", "/test/diff/upload/hihi.txt" },
            { "Content-Length", "7" },
            { "Content-Type", "text/plain" },
        },
        true,
        "Success",
        true,
        "/test/diff",
    },
    {
        "upload_to_different_location_PUT",
        "PUT /test/diff/hihi.txt HTTP/1.1\r\nHost: coucou\r\nContent-Length: 22\r\n\r\nCoucou je suis heureux",
        tUploadStrategy,
        Created,
        {
            { "Location", "/test/diff/upload/hihi.txt" },
            { "Content-Length", "7" },
            { "Content-Type", "text/plain" },
        },
        true,
        "Success",
        true,
        "/test/diff",
    },
    {
        "get_to_folder_with_file_name",
        "GET /test/test.txt HTTP/1.1\r\nHost: coucou\r\n\r\n",
        tGetFileStrategy,
        OK,
        {
            { "Content-Length", "33" },
            { "Content-Type", "text/plain" },
        },
        true,
        "This has a different mimetype...\n",
        false,
        "/test",
    },
};

INSTANTIATE_TEST_SUITE_P(
    ResponseBuildStateSuite,
    ResponseBuildStateFixture,
    ::testing::ValuesIn(ResponseBuildData),
    [](const testing::TestParamInfo< d_rbs > &info) {
        std::string name = std::get< tname >(info.param);
        return name;
    }
);

TEST_P(ResponseBuildStateFixture, CorrectStrategyTest) {
    strategies strat = std::get< ttype >(GetParam());
    if (dynamic_cast< CGIStrategy * >(_strategy) != 0)
        EXPECT_EQ(strat, tCGIStrategy);
    else if (dynamic_cast< DeleteStrategy * >(_strategy) != 0)
        EXPECT_EQ(strat, tDeleteStrategy);
    else if (dynamic_cast< ErrorStrategy * >(_strategy) != 0)
        EXPECT_EQ(strat, tErrorStrategy);
    else if (dynamic_cast< GetFileStrategy * >(_strategy) != 0)
        EXPECT_EQ(strat, tGetFileStrategy);
    else if (dynamic_cast< GetIndexStrategy * >(_strategy) != 0)
        EXPECT_EQ(strat, tGetIndexStrategy);
    else if (dynamic_cast< RedirectStrategy * >(_strategy) != 0)
        EXPECT_EQ(strat, tRedirectStrategy);
    else if (dynamic_cast< UploadStrategy< FakeServer, FakeRoute > * >(_strategy) != 0)
        EXPECT_EQ(strat, tUploadStrategy);
    else
        FAIL() << "Strategy was not found.";
}

TEST_P(ResponseBuildStateFixture, CorrectStatus) {
    HttpCode code = std::get< tstatus >(GetParam());
    EXPECT_EQ(code, _strategy->get_response().get_code());
}

TEST_P(ResponseBuildStateFixture, CorrectHeaders) {
    typedef std::map< std::string, std::string > map;
    map                                          correct      = std::get< theaders >(GetParam());
    const map                                   &test_headers = _strategy->get_response()._header;

    correct["Server"] = SERVER_SOFTWARE;
    ASSERT_NE(test_headers.find("Connection"), test_headers.end());
    correct["Connection"] = test_headers.at("Connection");

    map::const_iterator test_item;
    map::const_iterator correct_item;

    EXPECT_THAT(test_headers, ::testing::ContainerEq(correct));
}

TEST_P(ResponseBuildStateFixture, HaveBody) {
    bool correct = std::get< thavebody >(GetParam());

    EXPECT_EQ(_strategy->get_response().have_body(), correct);
}

TEST_P(ResponseBuildStateFixture, BodyValue) {
    typedef std::map< std::string, std::string > map;
    if (!_strategy->get_response().have_body())
        return;
    BodyWriter *bodywriter = _strategy->get_response().get_body();
    std::string body;
    while (!bodywriter->is_done())
        body += bodywriter->generate();
    const map &test_headers = _strategy->get_response()._header;
    if (dynamic_cast< BodyWriterLength * >(bodywriter) != 0) {
        if (test_headers.find("Content-Length") != test_headers.end()) {
            std::stringstream st;
            st << test_headers.find("Content-Length")->second;
            size_t len;
            st >> len;
            EXPECT_EQ(bodywriter->length(), len);
        }
    }
    const std::string &correct = std::get< tbody >(GetParam());
    EXPECT_EQ(body, correct);
}

TEST_P(ResponseBuildStateFixture, HaveCorrectRoot) {
    const std::string &target  = _request->_target;
    std::string        correct = std::get< troute >(GetParam());

    EXPECT_EQ(_server.getRoute(target).getLocation(), correct);
}

TEST(ResponseBuildStateSuite, NoRequest) {
    typedef ResponseBuildState< FakeServer, FakeRoute > template_test;
    FakeServer                                          server;
    error.disable();
    template_test test(0, 0, 80);
    EXPECT_EQ(test.get_status(), InternalServerError);
    error.enable();
}

// TODO: test for error with and with error file
