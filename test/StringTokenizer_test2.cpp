#include "StringTokenizer.hpp"
#include "gtest/gtest.h"

class StringTokenizerTestSuite : public ::testing::Test {
    protected:
        StringTokenizerTestSuite():
            TokenList1          ("servername|listen|root|index|maxBodySize", '|'),
            TokenList2          ("listen|root|index|maxBodySize", '|'),
            TokenList3          ("root|index|maxBodySize", '|'),
            TokenList4          ("index|maxBodySize", '|'),
            TokenList5          ("maxBodySize", '|'),
            TokenList6          ("", '|'),
            AdvancedTokenList1  (
                    "server|{|listen|80;|server_name|127.0.0.6;|root|www;|methods|GET;|index|index.html;|location|/images|{|methods|GET;|autoindex|on;|}|}",
                    '|'),
            AdvancedTokenList2  ("methods|GET;|autoindex|on;", '|') {}

        StringTokenizer TokenList1;
        StringTokenizer TokenList2;
        StringTokenizer TokenList3;
        StringTokenizer TokenList4;
        StringTokenizer TokenList5;
        StringTokenizer TokenList6;
        StringTokenizer AdvancedTokenList1;
        StringTokenizer AdvancedTokenList2;
};

TEST_F(StringTokenizerTestSuite, CountToken) {
    EXPECT_EQ(  TokenList1.countTokens(),   5ul);
    EXPECT_EQ(  TokenList2.countTokens(),   4ul);
    EXPECT_EQ(  TokenList3.countTokens(),   3ul);
    EXPECT_EQ(  TokenList4.countTokens(),   2ul);
    EXPECT_EQ(  TokenList5.countTokens(),   1ul);
    EXPECT_EQ(  TokenList6.countTokens(),   0ul);
}

TEST_F(StringTokenizerTestSuite, PeakToken) {
    EXPECT_EQ(  TokenList1.peakToken(), "servername");
    EXPECT_EQ(  TokenList2.peakToken(), "listen");
    EXPECT_EQ(  TokenList3.peakToken(), "root");
    EXPECT_EQ(  TokenList4.peakToken(), "index");
    EXPECT_EQ(  TokenList5.peakToken(), "maxBodySize");
    EXPECT_EQ(  TokenList6.peakToken(), "");
}

TEST_F(StringTokenizerTestSuite, nextToken) {
    EXPECT_EQ(  TokenList1.nextToken(), "servername");
    EXPECT_EQ(  TokenList1.countTokens(), 4ul);
    EXPECT_EQ(  TokenList2.nextToken(), "listen");
    EXPECT_EQ(  TokenList2.countTokens(), 3ul);
    EXPECT_EQ(  TokenList3.nextToken(), "root");
    EXPECT_EQ(  TokenList3.countTokens(), 2ul);
    EXPECT_EQ(  TokenList4.nextToken(), "index");
    EXPECT_EQ(  TokenList4.countTokens(), 1ul);
    EXPECT_EQ(  TokenList5.nextToken(), "maxBodySize");
    EXPECT_EQ(  TokenList5.countTokens(), 0ul);
    EXPECT_EQ(  TokenList6.nextToken(), "");
    EXPECT_EQ(  TokenList6.countTokens(), 0ul);
}

TEST_F(StringTokenizerTestSuite, nextTokenOfSeparator) {
    EXPECT_EQ(  TokenList1.nextToken(';'), "servername|listen|root|index|maxBodySize");
    EXPECT_EQ(  AdvancedTokenList1.nextToken('{'), "server");
    EXPECT_EQ(  AdvancedTokenList1.nextToken('}'),
            "|listen|80;|server_name|127.0.0.6;|root|www;|methods|GET;|index|index.html;|location|/images|{|methods|GET;|autoindex|on;");
    EXPECT_EQ(  AdvancedTokenList2.nextToken(';'), "methods|GET");
}

TEST_F( StringTokenizerTestSuite, nextTokenOfSeparatorAtPos) {
    EXPECT_EQ(  AdvancedTokenList1.nextToken(7ul), "server");
    EXPECT_EQ(  AdvancedTokenList1.nextToken((AdvancedTokenList1.remainingString().size() - 1)),
            "|listen|80;|server_name|127.0.0.6;|root|www;|methods|GET;|index|index.html;|location|/images|{|methods|GET;|autoindex|on;|}");
}

TEST_F( StringTokenizerTestSuite, filterToken) {}
