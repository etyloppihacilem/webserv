/* #############################################################################

               """          Response_test.cpp
        -\-    _|__
         |\___/  . \        Created on 07 Jun. 2024 at 11:45
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "HttpStatusCodes.hpp"
#include "Response.hpp"
#include "gtest/gtest.h"

TEST(ResponseTestSuite, generate_status_line) {
    Response test;

    test.set_code(OK);
    EXPECT_EQ(  test.get_code(), OK);
    EXPECT_EQ(  test.generate_status_line(), "HTTP/1.1 200 OK\r\n");
}
