/* #####################################################################################################################

               """          ResponseBuildState_test.cpp
        -\-    _|__
         |\___/  . \        Created on 28 Jul. 2024 at 16:08
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ResponseBuildState_test.hpp"
#include "FakeRoute.hpp"
#include "FakeServer.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "ResponseBuildState.hpp"
#include "gtest/gtest.h"
#include <exception>
#include <string>
#include <vector>

// TODO: implementer maxBodySize
// TODO: implementer UploadPath

FakeServer ResponseBuildStateFixture::_server;

std::vector<d_rbs> ResponseBuildData = {
    { "Test1", "Request", CGIStrategy, OK, {}, false, "" },
};

INSTANTIATE_TEST_SUITE_P(
    ResponseBuildStateSuite, ResponseBuildStateFixture, ::testing::ValuesIn(ResponseBuildData),
    [](const testing::TestParamInfo<d_rbs> &info) {
        std::string name = std::get<tname>(info.param);
        return name;
    }
);

TEST_P(ResponseBuildStateFixture, CorrectStrategyTest) {
    return;
}

TEST(ResponseBuildStateSuite, NoRequest) {
    typedef ResponseBuildState<FakeServer, FakeRoute> template_test;
    FakeServer                                        server;
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
}
