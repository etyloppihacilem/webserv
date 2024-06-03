/* #############################################################################

               """          BodyLength_test.cpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 15:31
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyLength.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "gtest/gtest.h"
#include <string>

TEST(BodyLengthTestSuite, Constructor) {
    std::string buffer = "Coucou je suis heureux";

    EXPECT_NO_THROW({
        BodyLength(0, buffer, "1234");
    });
}

TEST(BodyLengthTestSuite, BadConstructor) {
    std::string buffer = "Coucou je suis heureux";

    EXPECT_THROW({
        try {
            BodyLength(0, buffer, "coucou");
        } catch (HttpError &e) {
            EXPECT_EQ(e.get_code(), BadRequest);
            throw;
        } catch (std::exception &e) {
            throw;
        }
    }, HttpError);
    EXPECT_THROW({
        try {
            BodyLength(0, buffer, "cou2");
        } catch (HttpError &e) {
            EXPECT_EQ(e.get_code(), BadRequest);
            throw;
        } catch (std::exception &e) {
            throw;
        }
    }, HttpError);
    EXPECT_THROW({
        try {
            BodyLength(0, buffer, "-42");
        } catch (HttpError &e) {
            EXPECT_EQ(e.get_code(), BadRequest);
            throw;
        } catch (std::exception &e) {
            throw;
        }
    }, HttpError);
}
