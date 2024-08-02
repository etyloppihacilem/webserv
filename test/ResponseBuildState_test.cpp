/* #####################################################################################################################

               """          ResponseBuildState_test.cpp
        -\-    _|__
         |\___/  . \        Created on 28 Jul. 2024 at 16:08
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ResponseBuildState_test.hpp"
#include "BodyWriter.hpp"
#include "BodyWriterChunk.hpp"
#include "BodyWriterLength.hpp"
#include "CGIStrategy.hpp"
#include "DeleteStrategy.hpp"
#include "ErrorStrategy.hpp"
#include "FakeRoute.hpp"
#include "FakeServer.hpp"
#include "GetFileStrategy.hpp"
#include "GetIndexStrategy.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "RedirectStrategy.hpp"
#include "ResponseBuildState.hpp"
#include "UploadStrategy.hpp"
#include "todo.hpp"
#include "gtest/gtest.h"
#include <cstddef>
#include <exception>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// TODO: implementer maxBodySize
// TODO: implementer UploadPath

FakeServer ResponseBuildStateFixture::_server;

// Do not include default server header in header verification list.
// WARN: Do not test for CGI yet !! not implemented !!
std::vector<d_rbs> ResponseBuildData = {
    { "GET",
      "GET /index.html HTTP/1.1\r\nHost: coucou\r\n\r\n",
      tGetFileStrategy,
      OK,
      { { "Content-Type", "text/html" }, { "Content-Length", "102" } },
      true,
      "<body><h1>Coucou je suis heureux</h1><div>This file is there for test and demo purposes.</div></body>\n",
      false },
    { "GET_not_found",
      "GET /indox.html HTTP/1.1\r\nHost: coucou\r\n\r\n",
      tErrorStrategy,
      NotFound,
      { { "Content-Type", "text/html; charset=utf-8" }, { "Content-Length", "134" } },
      true,
      "<head><title>404 Error</title></head><body><h1>Error: 404 Not Found</h1><div>This error page was automatically "
      "generated.</div></body>",
      false },
    { "POST_create",
      "POST /test/upload.txt HTTP/1.1\r\nHost: coucou\r\nContent-Length: 22\r\n\r\nCoucou je suis heureux",
      tUploadStrategy,
      Created,
      { { "Location", "/test/upload.txt" } },
      false,
      "",
      true },
    { "DELETE",
      "DELETE /test/delete.png HTTP/1.1\r\nHost: coucou\r\nContent-Length: 22\r\n\r\nCoucou je suis heureux",
      tDeleteStrategy,
      NoContent,
      {},
      false,
      "",
      true },
    { "DELETE_not_found",
      "DELETE /test/not_upload.txt HTTP/1.1\r\nHost: coucou\r\nContent-Length: 22\r\n\r\nCoucou je suis heureux",
      tErrorStrategy,
      NotFound,
      { { "Content-Type", "text/html; charset=utf-8" }, { "Content-Length", "134" } },
      true,
      "<head><title>404 Error</title></head><body><h1>Error: 404 Not Found</h1><div>This error page was automatically "
      "generated.</div></body>",
      false },
    { "redirect",
      "GET /space_travel HTTP/1.1\r\nHost: coucou\r\n\r\n",
      tRedirectStrategy,
      MovedPermanently,
      { { "Location", "/star_wars/milky_way.html" } },
      false,
      "",
      false },
    { "redirect_discarding",
      "GET /space_travel/some/things HTTP/1.1\r\nHost: coucou\r\n\r\n",
      tRedirectStrategy,
      MovedPermanently,
      { { "Location", "/star_wars/milky_way.html" } },
      false,
      "",
      false },
    { "redirect_append_nothing",
      "GET /space_corridor HTTP/1.1\r\nHost: coucou\r\n\r\n",
      tRedirectStrategy,
      TemporaryRedirect,
      { { "Location", "http://space_corridor.fr/" } },
      false,
      "",
      false },
    { "redirect_append",
      "GET /space_corridor/search/ici?ici=coucou HTTP/1.1\r\nHost: coucou\r\n\r\n",
      tRedirectStrategy,
      TemporaryRedirect,
      { { "Location", "http://space_corridor.fr/search/ici?ici=coucou" } },
      false,
      "",
      false },
};

INSTANTIATE_TEST_SUITE_P(
    ResponseBuildStateSuite,
    ResponseBuildStateFixture,
    ::testing::ValuesIn(ResponseBuildData),
    [](const testing::TestParamInfo<d_rbs> &info) {
        std::string name = std::get<tname>(info.param);
        return name;
    }
);

TEST_P(ResponseBuildStateFixture, CorrectStrategyTest) {
    // _strategy
    strategies strat = std::get<ttype>(GetParam());
    if (dynamic_cast<CGIStrategy *>(_strategy) != 0)
        EXPECT_EQ(strat, tCGIStrategy);
    else if (dynamic_cast<DeleteStrategy *>(_strategy) != 0)
        EXPECT_EQ(strat, tDeleteStrategy);
    else if (dynamic_cast<ErrorStrategy *>(_strategy) != 0)
        EXPECT_EQ(strat, tErrorStrategy);
    else if (dynamic_cast<GetFileStrategy *>(_strategy) != 0)
        EXPECT_EQ(strat, tGetFileStrategy);
    else if (dynamic_cast<GetIndexStrategy *>(_strategy) != 0)
        EXPECT_EQ(strat, tGetIndexStrategy);
    else if (dynamic_cast<RedirectStrategy *>(_strategy) != 0)
        EXPECT_EQ(strat, tRedirectStrategy);
    else if (dynamic_cast<UploadStrategy *>(_strategy) != 0)
        EXPECT_EQ(strat, tUploadStrategy);
    else
        FAIL() << "Strategy was not found.";
}

TEST_P(ResponseBuildStateFixture, CorrectStatus) {
    HttpCode code = std::get<tstatus>(GetParam());
    EXPECT_EQ(code, _strategy->get_response().get_code());
}

TEST_P(ResponseBuildStateFixture, CorrectHeaders) {
    typedef std::map<std::string, std::string> map;
    map                                        correct      = std::get<theaders>(GetParam());
    const map                                 &test_headers = _strategy->get_response()._header;

    correct["Server"] = SERVER_SOFTWARE;

    map::const_iterator test_item;
    map::const_iterator correct_item;

    EXPECT_EQ(correct.size(), test_headers.size());
    for (correct_item = correct.begin(); correct_item != correct.end(); correct_item++) {
        test_item = test_headers.find(correct_item->first);
        if (test_item == test_headers.end())
            FAIL() << "This header was expected :\n"
                   << correct_item->first << ": " << correct_item->second << "\n...not found.";
        if (test_item != test_headers.end())
            EXPECT_EQ(test_item->second, correct_item->second);
    }
    for (test_item = test_headers.begin(); test_item != test_headers.end(); test_item++) {
        correct_item = correct.find(test_item->first);
        if (correct_item == correct.end())
            FAIL() << "This header was found :\n"
                   << test_item->first << ": " << test_item->second << "\n...not expected.";
    } // double verification is to display clearly which one is missing.
}

TEST_P(ResponseBuildStateFixture, HaveBody) {
    bool correct = std::get<thavebody>(GetParam());

    EXPECT_EQ(_strategy->get_response().have_body(), correct);
}

TEST_P(ResponseBuildStateFixture, BodyValue) {
    typedef std::map<std::string, std::string> map;
    if (!_strategy->get_response().have_body())
        return;
    BodyWriter *bodywriter = _strategy->get_response().get_body();
    std::string body;
    if (dynamic_cast<BodyWriterChunk *>(bodywriter) != 0) {
        while (!bodywriter->is_done())
            body += bodywriter->generate();
    } else if (dynamic_cast<BodyWriterLength *>(bodywriter) != 0) {
        body = bodywriter->generate();
    } else {
        FAIL() << "Unknown body type.";
        return;
    }
    const map &test_headers = _strategy->get_response()._header;
    if (test_headers.find("Content-Length") != test_headers.end()) {
        std::stringstream st;
        st << test_headers.find("Content-Length")->second;
        size_t len;
        st >> len;
        EXPECT_EQ(bodywriter->length(), len);
    }
    const std::string &correct = std::get<tbody>(GetParam());
    EXPECT_EQ(body, correct);
}

TEST(ResponseBuildStateSuite, NoRequest) {
    typedef ResponseBuildState<FakeServer, FakeRoute> template_test;
    FakeServer                                        server;
    error.disable();
    EXPECT_THROW(
        {
            try {
                template_test test(0, 0, server);
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), InternalServerError);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    error.enable();
}

// TODO: test for upload with create and append
// TODO: test for error with and without error file
