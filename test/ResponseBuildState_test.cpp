/* #####################################################################################################################

               """          ResponseBuildState_test.cpp
        -\-    _|__
         |\___/  . \        Created on 28 Jul. 2024 at 16:08
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ResponseBuildState_test.hpp"
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
#include "gtest/gtest.h"
#include <exception>
#include <string>
#include <vector>

// TODO: implementer maxBodySize
// TODO: implementer UploadPath

FakeServer ResponseBuildStateFixture::_server;

std::vector<d_rbs> ResponseBuildData = {
    { "Test1",
      "GET /index.html HTTP/1.1\r\nHost: coucou\r\n\r\n",
      tGetFileStrategy,
      OK,
      { { "Content-Type", "text/html" } },
      true,
      "<body><h1>Coucou je suis heureux</h1><div>This file is there for test and demo purposes.</div></body>" },
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
