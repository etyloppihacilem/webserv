/* #############################################################################

               """          BodyChunk_test.cpp
        -\-    _|__
         |\___/  . \        Created on 04 Jun. 2024 at 16:28
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyChunk.hpp"
#include "gtest/gtest.h"
#include <cstddef>
#include <ostream>
#include <string>
#include <unistd.h>
#include "Logger.hpp"

TEST(BodyChunkTestSuite, is_hex) {
    std::string buffer = "ca32\r\n";
    BodyChunk   test(0, buffer);

    EXPECT_TRUE(test.is_hex('0'));
    EXPECT_TRUE(test.is_hex('1'));
    EXPECT_TRUE(test.is_hex('2'));
    EXPECT_TRUE(test.is_hex('3'));
    EXPECT_TRUE(test.is_hex('4'));
    EXPECT_TRUE(test.is_hex('5'));
    EXPECT_TRUE(test.is_hex('6'));
    EXPECT_TRUE(test.is_hex('7'));
    EXPECT_TRUE(test.is_hex('8'));
    EXPECT_TRUE(test.is_hex('9'));
    EXPECT_TRUE(test.is_hex('a'));
    EXPECT_TRUE(test.is_hex('b'));
    EXPECT_TRUE(test.is_hex('c'));
    EXPECT_TRUE(test.is_hex('d'));
    EXPECT_TRUE(test.is_hex('e'));
    EXPECT_TRUE(test.is_hex('f'));
    EXPECT_TRUE(test.is_hex('A'));
    EXPECT_TRUE(test.is_hex('B'));
    EXPECT_TRUE(test.is_hex('C'));
    EXPECT_TRUE(test.is_hex('D'));
    EXPECT_TRUE(test.is_hex('E'));
    EXPECT_TRUE(test.is_hex('F'));
    EXPECT_FALSE(   test.is_hex(' '));
    EXPECT_FALSE(   test.is_hex('\n'));
    EXPECT_FALSE(   test.is_hex('\r'));
    EXPECT_FALSE(   test.is_hex('\t'));
    EXPECT_FALSE(   test.is_hex('g'));
    EXPECT_FALSE(   test.is_hex('G'));
    EXPECT_FALSE(   test.is_hex('@'));
    EXPECT_FALSE(   test.is_hex('`'));
    EXPECT_FALSE(   test.is_hex('/'));
}

TEST(BodyChunkTestSuite, init_chunk) {
    std::string buffer = "ca32\r\ncoucou";
    BodyChunk   test(0, buffer);

    info.log() << "Warning are normal for this test." << std::endl;
    EXPECT_EQ(test._bytes_remaining, (size_t) 0);
    test._bytes_remaining   = 2;
    test.   init_chunk();
    test._bytes_remaining   = 0;
    test.   init_chunk();
    EXPECT_EQ(  test._bytes_remaining, (size_t) 0xca32);
    EXPECT_EQ(  test._buffer, "coucou");
    test.   init_chunk();
    test._bytes_remaining = 0;
    buffer = "CA32\r\nCOUCOU";
    test.   init_chunk();
    EXPECT_EQ(  test._bytes_remaining, (size_t) 0xca32);
    EXPECT_EQ(  test._buffer, "COUCOU");
}

TEST(BodyChunkTestSuite, init_chunk_end) {
    std::string buffer
        = "16\r\nCoucou je suis heureux\r\n41\r\n"
          " de pouvoir tester le comportement d'un body_length et de compren\r\n2b\r\n"
          "dre comment pouvoir lire de facon certaine.\r\n\r\n";
    BodyChunk test(0, buffer);

    info.log() << "Warning are normal for this test." << std::endl;
    test._bytes_remaining = 0;
    test.init_chunk();
    EXPECT_EQ(test._bytes_remaining, (size_t) 0x16);
    test.   init_chunk();
    test._bytes_remaining = 0;

    buffer = "0\r\n""trailer section\r\n""\r\n";

    test.   init_chunk();
    EXPECT_EQ(test._bytes_remaining, (size_t) 0x0);
    EXPECT_TRUE(test.is_done());
}

TEST(BodyChunkTestSuite, init_chunk_bad) {
    std::string buffer = "zxswABC\r\n" "2a\r\n";
    BodyChunk   test(0, buffer);

    test.init_chunk();
    EXPECT_EQ(test._bytes_remaining, (size_t) 0x2a);
}

TEST(BodyChunkTestSuite, init_chunk_none) {
    std::string buffer = "zxsw\r\n";
    BodyChunk   test(0, buffer);

    test.init_chunk();
    EXPECT_EQ(  test._bytes_remaining, (size_t) 0);
    EXPECT_EQ(  test._buffer, "");
}

// suspended because of change of approach for reading with chunks
/* TEST(BodyChunkTestSuite, read_body) {
    static const char buf[]
        = "Coucou je suis heureux de pouvoir tester le comportement d'un body_length et de compren"
          "dre comment pouvoir lire de facon certaine.";
    std::string tmp1(buf);
    std::string tmp2;
    int         fd[2];

    if (pipe(fd) < 0)
        GTEST_FATAL_FAILURE_("Pipe creation failed");

    std::string buffer = "";
    BodyChunk   test(fd[0], buffer);

    write(fd[1], buf, 130);
    for (size_t i = 1; i <= 130 / BUFFER_SIZE; i++) {
        tmp2 = tmp1.substr(0, i * BUFFER_SIZE);
        test.read_body();
        EXPECT_EQ(  tmp2,   test._buffer);
        EXPECT_EQ(  tmp2,   buffer);
    }
} */

TEST(BodyChunkTestSuite, get) {
    static const char buf[]
        =
            "16\r\nCoucou je suis heureux\r\n41\r\n de pouvoir tester le comportement d'un body_length et de compren\r\n2b\r\ndre comment pouvoir lire de facon certaine.\r\n0\r\ntrailing\r\n\r\n";
    int fd[2];

    if (pipe(fd) < 0)
        GTEST_FATAL_FAILURE_("Pipe creation failed");

    std::string buffer = "";
    BodyChunk   test(fd[0], buffer);
    std::string tmp1
        =
            "Coucou je suis heureux de pouvoir tester le comportement d'un body_length et de comprendre comment pouvoir lire de facon certaine.";
    std::string ret = "not good";
    size_t      i   = 1000;

    write(fd[1], buf, sizeof(buf) - 1);
    while (!test.is_done() && --i) {
        ret = test.get();
    }
    if (!i)
        GTEST_FATAL_FAILURE_("Infinite loop detected.");
    EXPECT_EQ(  tmp1,           ret);
    EXPECT_EQ(  &test._body,    &test.get());
    EXPECT_EQ(  tmp1,           test._body);
    EXPECT_TRUE(test.is_done());
}

TEST(BodyChunkTestSuite, pop) {
    static const char buf[]
        =
            "16\r\nCoucou je suis heureux\r\n41\r\n de pouvoir tester le comportement d'un body_length et de compren\r\n2b\r\ndre comment pouvoir lire de facon certaine.\r\n0\r\ntrailing\r\n\r\n";
    int fd[2];

    if (pipe(fd) < 0)
        GTEST_FATAL_FAILURE_("Pipe creation failed");

    std::string buffer = "";
    BodyChunk   test(fd[0], buffer);
    std::string tmp1
        =
            "Coucou je suis heureux de pouvoir tester le comportement d'un body_length et de comprendre comment pouvoir lire de facon certaine.";
    std::string ret = "not good";
    size_t      i   = 1000;
    size_t      check;

    write(fd[1], buf, sizeof(buf) - 1);
    while (!test.is_done() && --i) {
        ret     = test.pop();
        check   = tmp1.find(ret);
        EXPECT_EQ(  check,  (size_t) 0);
        EXPECT_EQ(  ret,    tmp1.substr(0, ret.length()));
        tmp1 = tmp1.substr(ret.length(), tmp1.length() - ret.length());
    }
    if (!i)
        GTEST_FATAL_FAILURE_("Infinite loop detected.");
    EXPECT_TRUE(test.is_done());
}
