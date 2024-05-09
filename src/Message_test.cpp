/* #############################################################################

               """          Message_test.cpp
        -\-    _|__
         |\___/  . \        Created on 07 May. 2024 at 11:13
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "gtest/gtest.h"
#include <exception>
#include "HttpError.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Message.hpp"

/*
 * Can access private elements of Message.
 * LSP is wrong
 * */
TEST(MessageTest, ParseMethodTestExpectedOK) {
    Message test;

    EXPECT_EQ(test.parse_method("GET", 3),  GET);
    EXPECT_EQ(test.parse_method("POST", 4), POST);
    EXPECT_EQ(test.parse_method("DELETE", 6), DELETE);
    EXPECT_EQ(test.parse_method("GET /var/srcs HTTP/1.1", 3),  GET);
    EXPECT_EQ(test.parse_method("POST /var/srcs HTTP/1.1", 4), POST);
    EXPECT_EQ(test.parse_method("DELETE /var/srcs HTTP/1.1", 6), DELETE);
}

TEST(MessageTest, ParseMethodTestExpectedFail) {
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
