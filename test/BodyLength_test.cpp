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
#include "Logger.hpp"
#include "todo.hpp"
#include "gtest/gtest.h"
#include <cstddef>
#include <exception>
#include <string>
#include <unistd.h>

TEST(BodyLengthTestSuite, Constructor) {
    std::string buffer = "Coucou je suis heureux";

    ASSERT_NO_THROW({ BodyLength(0, buffer, "1234"); });

    BodyLength test(0, buffer, "1234");

    EXPECT_EQ(test._buffer, buffer);
    EXPECT_EQ(test.is_done(), false);
    EXPECT_EQ(test._length, (size_t) 1234);
    EXPECT_EQ(test._read_length, (size_t) buffer.length());
}

TEST(BodyLengthTestSuite, BadConstructor) {
    info.disable();
    std::string buffer = "Coucou je suis heureux";

    EXPECT_THROW(
        {
            try {
                BodyLength(0, buffer, "coucou");
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), BadRequest);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    EXPECT_THROW(
        {
            try {
                BodyLength(0, buffer, "cou2");
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), BadRequest);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    EXPECT_THROW(
        {
            try {
                BodyLength(0, buffer, "-42");
            } catch (HttpError &e) {
                EXPECT_EQ(e.get_code(), BadRequest);
                throw;
            } catch (std::exception &e) {
                throw;
            }
        },
        HttpError
    );
    info.enable();
}

TEST(BodyLengthTestSuite, read_body) {
    static const char buf[]
        = "Coucou je suis heureux de pouvoir tester le comportement d'un body_length et de comprendre comment pouvoir "
          "lire de facon certaine.";
    std::string tmp1(buf);
    std::string tmp2;
    int         fd[2];

    if (pipe(fd) < 0)
        GTEST_FATAL_FAILURE_("Pipe creation failed");

    std::string nothing;
    BodyLength  test(fd[0], nothing, "130");

    write(fd[1], buf, 130);
    for (size_t i = 1; i - 1 <= 130 / BUFFER_SIZE; i++) {
        tmp2 = tmp1.substr(0, i * BUFFER_SIZE);
        test.read_body();
        test.get();
        EXPECT_EQ(tmp2, test._body);
        EXPECT_EQ((size_t) i * BUFFER_SIZE > 130 ? 130 : (size_t) i * BUFFER_SIZE, test._read_length);
    }
    EXPECT_TRUE(test.is_done());
}

TEST(BodyLengthTestSuite, get) {
    static const char buf[]
        = "Coucou je suis heureux de pouvoir tester le comportement d'un body_length et de comprendre comment pouvoir "
          "lire de facon certaine.";
    std::string tmp1(buf);
    std::string tmp2;
    std::string res;
    int         fd[2];

    if (pipe(fd) < 0)
        GTEST_FATAL_FAILURE_("Pipe creation failed");

    std::string buffer = "";
    BodyLength  test(fd[0], buffer, "130");

    write(fd[1], buf, 130);
    for (size_t i = 1; i - 1 <= 130 / BUFFER_SIZE; i++) {
        tmp2 = tmp1.substr(0, i * BUFFER_SIZE);
        test.read_body();
        res = test.get();
        EXPECT_EQ(tmp2, res);
        EXPECT_EQ((size_t) i * BUFFER_SIZE > 130 ? 130 : (size_t) i * BUFFER_SIZE, test._read_length);
    }
    EXPECT_TRUE(test.is_done());
}

TEST(BodyLengthTestSuite, pop) {
    static const char buf[]
        = "Coucou je suis heureux de pouvoir tester le comportement d'un body_length et de comprendre comment pouvoir "
          "lire de facon certaine.";
    std::string tmp1(buf);
    std::string tmp2;
    std::string res;
    int         fd[2];

    if (pipe(fd) < 0)
        GTEST_FATAL_FAILURE_("Pipe creation failed");

    std::string buffer = "";
    BodyLength  test(fd[0], buffer, "130");

    write(fd[1], buf, 130);
    for (size_t i = 1; i - 1 <= 130 / BUFFER_SIZE; i++) {
        tmp2 = tmp1.substr((i - 1) * BUFFER_SIZE, BUFFER_SIZE);
        test.read_body();
        res = test.pop();
        EXPECT_EQ(tmp2, res);
        EXPECT_EQ((size_t) i * BUFFER_SIZE > 130 ? 130 : (size_t) i * BUFFER_SIZE, test._read_length);
    }
    EXPECT_TRUE(test.is_done());
    EXPECT_EQ(test._buffer, "");
}
