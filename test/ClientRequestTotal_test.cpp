/* #####################################################################################################################

               """          ClientRequestTotal_test.cpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 17:07
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ClientRequest.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ReadState.hpp"
#include "todo.hpp"
#include "gtest/gtest.h"
#include <cassert>
#include <cstddef>
#include <map>
#include <ostream>
#include <string>
#include <tuple>
#include <unistd.h>
#include <vector>

typedef enum e_total_index {
    tname = 0,
    tdata,
    ttarget,
    tmethod,
    thavebody,
    tbody,
    theaders,
    tstatus,
    tport,
    tqs,
} t_ti;

typedef std::tuple<std::string,             ///< Name
        std::string,                        ///< Request data (including body and all stuff)
        std::string,                        ///< Target
        HttpMethod,                         ///< Method
        bool,                               ///< HaveBody
        std::string,                        ///< Body after parsing (will be used to test Body objects)
        std::map<std::string, std::string>, ///< Headers
        HttpCode,                           ///< Status
        int,                                ///< port ???
        std::string                         ///< QueryString (things after ?)
        > TotalRequest;

class TotalRequestFixture: public ::testing::TestWithParam<TotalRequest> {
    public:
        TotalRequestFixture():
            _test   (0),
            _fd     {0, 0} {}

        void SetUp() override {
            if (pipe(_fd))
                GTEST_FATAL_FAILURE_("Pipe error");

            const std::string &raw = std::get<tdata>(GetParam());

            if (write(_fd[1], raw.c_str(), raw.length()) < 0)
                GTEST_FATAL_FAILURE_("Write in pipe failure");
            close(_fd[1]);
            _test = new ReadState(_fd[0]);
        };

        void TearDown() override {
            if (_test)
                delete _test;
            if (_fd[0] != 0)
                close(_fd[0]);
        };

        void test_process() {
            size_t i = 0;

            while (!_test->process() && i < 5000)
                i++;
            if (i >= 5000)
                GTEST_FATAL_FAILURE_("Infinite loop detected.");
            ASSERT_TRUE(_test->process());
            _request = _test->get_client_request();
            ASSERT_NE(_request, (void *) 0);
        }

    protected:
        ReadState       *_test;
        ClientRequest   *_request;

    private:
        int         _fd[2];
        std::string _buffer;
};

std::vector<TotalRequest> TotalRequestData = {
    {
        "Basic_GET", "GET /helloworld.html?hihi=ahah HTTP/1.1\r\nHost: 127.0.0.1\r\nName: fireTesting/1.0\r\n\r\n",
        "/helloworld.html", GET, false, "",{
            {
                "Host", "127.0.0.1"
            },{
                "Name", "fireTesting/1.0"
            }
        }, OK, 80, "hihi=ahah"
    }
};

INSTANTIATE_TEST_SUITE_P(TotalRequestSuite,
        TotalRequestFixture,
        ::testing::ValuesIn(TotalRequestData),
        [](const testing::TestParamInfo<TotalRequest> &info)
{
    // Can use info.param here to generate the test suffix
    std::string name = std::get<tname>(info.param);
    return name;
});

TEST_P(TotalRequestFixture, TargetTest) {
    test_process();

    const std::string &correct = std::get<ttarget>(GetParam());

    EXPECT_EQ(_request->get_target(), correct);
}

TEST_P(TotalRequestFixture, MethodTest) {
    test_process();

    HttpMethod correct = std::get<tmethod>(GetParam());

    EXPECT_EQ(_request->get_method(), correct);
}

TEST_P(TotalRequestFixture, BodyTest) {
    const std::string &correct = std::get<tbody>(GetParam());

    if (!std::get<thavebody>(GetParam())) {
        info.log() << "If this message is displayed, warning is normal for this test." << std::endl;
        EXPECT_EQ(_request->get_body(), (void *) 0);
        return;
    }
    (void) correct;
    // TODO: tester quand on aura vraiment un body.
}

TEST_P(TotalRequestFixture, HeadersTest) {
    typedef std::map<std::string, std::string> map;

    const map   &correct        = std::get<theaders>(GetParam());
    const map   &test_headers   = _request->get_header();

    if (isRedirection(std::get<tstatus>(GetParam()))) {
        ASSERT_NE(correct.find("Location"), correct.end()); // pas de location pour vérifier la redirection dans le

        // test.

        map::const_iterator loc = test_headers.find("Location");

        ASSERT_NE(loc, test_headers.end());
        EXPECT_EQ(correct.find("Location")->second, loc->second);
        return;
    }

    map::const_iterator test_item;
    map::const_iterator correct_item;

    EXPECT_EQ(correct.size(), test_headers.size());
    for (correct_item = correct.begin(); correct_item != correct.end(); correct_item++) {
        test_item = test_headers.find(correct_item->first);
        EXPECT_NE(test_item, test_headers.end());
        if (test_item != test_headers.end())
            EXPECT_EQ(test_item->second, correct_item->second);
    }
    for (test_item = correct.begin(); test_item != correct.end(); test_item++) {
        correct_item = correct.find(test_item->first);
        EXPECT_NE(correct_item, correct.end());
        if (correct_item != test_headers.end())
            EXPECT_EQ(correct_item->second, test_item->second);
    } // double verification is to display clearly which one is missing.
}
