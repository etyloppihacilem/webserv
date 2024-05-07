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
TEST(MessageTest, ParseMethodTest) {
    Message test;

    EXPECT_EQ(test.parse_method("GET"),  GET);
    EXPECT_EQ(test.parse_method("POST"), POST);
    EXPECT_EQ(test.parse_method("DELETE"), DELETE);
    EXPECT_THROW({
        try {
            test.parse_method("(&)");
        } catch (HttpError &e) {
            EXPECT_EQ(e.get_code(), NotImplemented);
            throw;
        } catch (std::exception) {
            throw;
        }
    }, HttpError);
    EXPECT_THROW({
        try {
            test.parse_method("");
        } catch (HttpError &e) {
            EXPECT_EQ(e.get_code(), NotImplemented);
            throw;
        } catch (std::exception) {
            throw;
        }
    }, HttpError);
}
