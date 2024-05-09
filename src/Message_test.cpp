/* #############################################################################

               """          Message_test.cpp
        -\-    _|__
         |\___/  . \        Created on 07 May. 2024 at 11:13
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "gtest/gtest.h"
#include <exception>
#include <iostream>
#include <ostream>
#include <string>
#include "HttpError.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "MessageTest.hpp"
#include "Message.hpp"

/*
 * Can access private elements of Message.
 * LSP is wrong
 * */
TEST(MessageTestSuite, ParseMethodTestExpectedOK) {
    Message test;

    EXPECT_EQ(test.parse_method("GET", 3),  GET);
    EXPECT_EQ(test.parse_method("POST", 4), POST);
    EXPECT_EQ(test.parse_method("DELETE", 6), DELETE);
    EXPECT_EQ(test.parse_method("GET /var/srcs HTTP/1.1", 3),  GET);
    EXPECT_EQ(test.parse_method("POST /var/srcs HTTP/1.1", 4), POST);
    EXPECT_EQ(test.parse_method("DELETE /var/srcs HTTP/1.1", 6), DELETE);
}

TEST(MessageTestSuite, ParseMethodTestExpectedFail) {
    Message test;

    EXPECT_THROW({
        try {
            test.parse_method("(&)", 3);
        } catch (HttpError &e) {
            EXPECT_EQ(e.get_code(), NotImplemented);
            throw;
        } catch (std::exception) {
            throw;
        }
    }, HttpError);
    EXPECT_THROW({
        try {
            test.parse_method("DELETED", 7);
        } catch (HttpError &e) {
            EXPECT_EQ(e.get_code(), NotImplemented);
            throw;
        } catch (std::exception) {
            throw;
        }
    }, HttpError);
    EXPECT_THROW({
        try {
            test.parse_method("G", 3);
        } catch (HttpError &e) {
            EXPECT_EQ(e.get_code(), NotImplemented);
            throw;
        } catch (std::exception) {
            throw;
        }
    }, HttpError);
    EXPECT_THROW({
        try {
            test.parse_method("", 0);
        } catch (HttpError &e) {
            EXPECT_EQ(e.get_code(), NotImplemented);
            throw;
        } catch (std::exception) {
            throw;
        }
    }, HttpError);
    EXPECT_THROW({
        try {
            test.parse_method("GET /var/srcs HTTP/1.1", 4);
        } catch (HttpError &e) {
            EXPECT_EQ(e.get_code(), NotImplemented);
            throw;
        } catch (std::exception) {
            throw;
        }
    }, HttpError);
    EXPECT_THROW({
        try {
            test.parse_method("DELETE /var/srcs HTTP/1.1", 2);
        } catch (HttpError &e) {
            EXPECT_EQ(e.get_code(), NotImplemented);
            throw;
        } catch (std::exception) {
            throw;
        }
    }, HttpError);
    EXPECT_THROW({
        try {
            test.parse_method("thismethodiswaytoolong", 22);
        } catch (HttpError &e) {
            EXPECT_EQ(e.get_code(), NotImplemented);
            throw;
        } catch (std::exception) {
            throw;
        }
    }, HttpError);
}

TEST_P(MessageTest, ParseTargetTestExpectedOK) {
    std::string line = "METHOD ";
    size_t      pos  = sizeof("METHOD ");
    std::string params[3];

    t_test_target tmp = GetParam();

    params[0] = tmp.c1;
    params[1] = tmp.c2;
    params[2] = tmp.c3;
    std::cout << tmp.c1 << std::endl;
    line     += params[0];
    if (params[1] == "BadRequest") {
        EXPECT_THROW({
                try {
                    test.parse_target(line, pos);
                } catch (HttpError &e) {
                    EXPECT_EQ(e.get_code(), BadRequest);
                    throw;
                } catch (std::exception) {
                    throw;
                }
            }, HttpError);
    } else if (params[1] == "URITooLong") {
        EXPECT_THROW({
                try {
                    test.parse_target(line, pos);
                } catch (HttpError &e) {
                    EXPECT_EQ(e.get_code(), URITooLong);
                    throw;
                } catch (std::exception) {
                    throw;
                }
            }, HttpError);
    } else if (params[1] == "MovedPermanently") {
        EXPECT_THROW({
                try {
                    test.parse_target(line, pos);
                } catch (HttpError &e) {
                    EXPECT_EQ(e.get_code(), MovedPermanently);
                    EXPECT_EQ(e.what(), params[2]);
                    throw;
                } catch (std::exception) {
                    throw;
                }
            }, HttpError);
    } else {
        EXPECT_NO_THROW(test.parse_target(line, pos));
        EXPECT_EQ(test._target, params[1]);
        if (params[2] != "")
            EXPECT_EQ(test._header["Host"], params[2]);
        else
            EXPECT_EQ(test._header.find("Host"), test._header.end());
    }
}
static const t_test_target MessageTargetSuiteValues[] {
    {
        "normal", "/dev HTTP/1.1", "/dev", ""
    },
};
INSTANTIATE_TEST_SUITE_P(MessageTargetSuite, MessageTest, ::testing::ValuesIn(MessageTargetSuiteValues), [](const testing::TestParamInfo<t_test_target>& info) {
      // Can use info.param here to generate the test suffix
      std::string name = info.param.name;
      return name;
    });
