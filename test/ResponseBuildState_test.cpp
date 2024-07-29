/* #####################################################################################################################

               """          ResponseBuildState_test.cpp
        -\-    _|__
         |\___/  . \        Created on 28 Jul. 2024 at 16:08
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "FakeRoute.hpp"
#include "FakeServer.hpp"
#include "HttpStatusCodes.hpp"
#include "ResponseBuildState_test.hpp"
#include "gtest/gtest.h"
#include <string>
#include <vector>

// TODO: implementer maxBodySize
// TODO: implementer UploadPath

FakeServer ResponseBuildStateFixture::_server;

TEST_P(ResponseBuildStateFixture, CorrectStrategyTest) {
    return;
}

std::vector<d_rbs> ResponseBuildData = {
    {
        "Test1", "Request", aucune, OK,{                                    }, false, ""
    },
};

INSTANTIATE_TEST_SUITE_P(ResponseBuildStateSuite,
        ResponseBuildStateFixture,
        ::testing::ValuesIn(ResponseBuildData),
        [](const testing::TestParamInfo<d_rbs> &info)
{
    std::string name = std::get<tname>(info.param);
    return name;
});
