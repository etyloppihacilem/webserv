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
#include <string>

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

    EXPECT_EQ(test._bytes_remaining, (size_t) 0);
    test._bytes_remaining = 2;
    EXPECT_FALSE(test.init_chunk());
    test._bytes_remaining = 0;
    EXPECT_TRUE(test.init_chunk());
    EXPECT_EQ(  test._bytes_remaining, (size_t) 0xca32);
    EXPECT_EQ(  test._buffer, "coucou");
    EXPECT_FALSE(test.init_chunk());
    test._bytes_remaining   = 0;
    buffer                  = "CA32\r\nCOUCOU";
    EXPECT_TRUE(test.init_chunk());
    EXPECT_EQ(  test._bytes_remaining, (size_t) 0xca32);
    EXPECT_EQ(  test._buffer, "COUCOU");
}

TEST(BodyChunkTestSuite, init_chunk_bad) {
    std::string buffer = "zxswABC\r\n" "2a\r\n";
    BodyChunk   test(0, buffer);

    EXPECT_TRUE(test.init_chunk());
    EXPECT_EQ(test._bytes_remaining, (size_t) 0x2a);
}

TEST(BodyChunkTestSuite, init_chunk_none) {
    std::string buffer = "zxsw\r\n";
    BodyChunk   test(0, buffer);

    EXPECT_FALSE(test.init_chunk());
    EXPECT_EQ(  test._bytes_remaining, (size_t) 0);
    EXPECT_EQ(  test._buffer, "");
}
