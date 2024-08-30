#include "Logger.hpp"
#include "StringTokenizer.hpp"
#include "gtest/gtest.h"
#include <cstddef>
#include <string>
#include <tuple>

// NOTE: could refactor ParamTestSuite, overcomplicated for what is needed.
typedef std::string Name;
typedef std::string Input;
typedef char        Delim;
typedef std::string TokenizedString;
typedef std::string Token;
typedef std::size_t Count;

typedef std::tuple< Name, Input, Delim, TokenizedString > InitTest;

static const InitTest ListOfParam[]{
    { "OK", "----ffgsd skdf-dd-dd-/fsh--- - /df---", '-', "ffgsd skdf-dd-dd-/fsh- - /df" },
    { "OK_NoDelimInInput", "----ffgsd skdf-dd-dd-/fsh--- - /df---", 'x', "----ffgsd skdf-dd-dd-/fsh--- - /df---" },
    { "KO_EmptyInput", "", '-', "" },
};

class StringTokenizerParamTestSuite : public ::testing::TestWithParam< InitTest > {
    public:
        StringTokenizerParamTestSuite() : tokenizedTest(std::get< 1 >(GetParam()), std::get< 2 >(GetParam())) {}

        struct PrintToStringTestName {
                std::string operator()(const testing::TestParamInfo< InitTest > &info) const {
                    auto test_case = static_cast< InitTest >(info.param);
                    return std::get< 0 >(test_case);
                }
        };

    protected:
        StringTokenizer tokenizedTest;
};

TEST_P(StringTokenizerParamTestSuite, ObjectInstanciationTest) {
    auto [name, input, delim, tokenizedString] = GetParam();
    ASSERT_EQ(tokenizedString, tokenizedTest.remainingString());
    if (tokenizedTest.remainingString().empty()) {
        ASSERT_TRUE(name.find("KO") == 0);
        ASSERT_TRUE(name.find("OK") == std::string::npos);
        return;
    }
    ASSERT_EQ(delim, tokenizedTest.delimiter());
}

INSTANTIATE_TEST_SUITE_P(
    StringTokenizerTests,
    StringTokenizerParamTestSuite,
    ::testing::ValuesIn(ListOfParam),
    StringTokenizerParamTestSuite::PrintToStringTestName()
);

class StringTokenizerTestSuite : public ::testing::Test {
    protected:
        StringTokenizerTestSuite() :
            TokenList1("servername|listen|root|index|maxBodySize", '|'),
            TokenList2("listen|root|index|maxBodySize", '|'),
            TokenList3("root|index|maxBodySize", '|'),
            TokenList4("index|maxBodySize", '|'),
            TokenList5("maxBodySize", '|'),
            TokenList6("", '|'),
            AdvancedTokenList1(
                "server|{|listen|80;|server_name|127.0.0.6;|root|www;|methods|GET;|index|index.html;|location|/"
                "images|{|methods|GET;|autoindex|on;|}|}",
                '|'
            ),
            AdvancedTokenList2("methods|GET;|autoindex|on;", '|') {
            error.disable();
        }

        ~StringTokenizerTestSuite() { error.enable(); }

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
    EXPECT_EQ(TokenList1.countTokens(), 5ul);
    EXPECT_EQ(TokenList2.countTokens(), 4ul);
    EXPECT_EQ(TokenList3.countTokens(), 3ul);
    EXPECT_EQ(TokenList4.countTokens(), 2ul);
    EXPECT_EQ(TokenList5.countTokens(), 1ul);
    EXPECT_EQ(TokenList6.countTokens(), 0ul);
}

TEST_F(StringTokenizerTestSuite, PeakToken) {
    EXPECT_EQ(TokenList1.peakToken(), "servername");
    EXPECT_EQ(TokenList2.peakToken(), "listen");
    EXPECT_EQ(TokenList3.peakToken(), "root");
    EXPECT_EQ(TokenList4.peakToken(), "index");
    EXPECT_EQ(TokenList5.peakToken(), "maxBodySize");
    EXPECT_EQ(TokenList6.peakToken(), "");
}

TEST_F(StringTokenizerTestSuite, nextToken) {
    EXPECT_EQ(TokenList1.nextToken(), "servername");
    EXPECT_EQ(TokenList1.countTokens(), 4ul);
    EXPECT_EQ(TokenList1.remainingString(), "listen|root|index|maxBodySize");
    EXPECT_EQ(TokenList2.nextToken(), "listen");
    EXPECT_EQ(TokenList2.countTokens(), 3ul);
    EXPECT_EQ(TokenList2.remainingString(), "root|index|maxBodySize");
    EXPECT_EQ(TokenList3.nextToken(), "root");
    EXPECT_EQ(TokenList3.countTokens(), 2ul);
    EXPECT_EQ(TokenList3.remainingString(), "index|maxBodySize");
    EXPECT_EQ(TokenList4.nextToken(), "index");
    EXPECT_EQ(TokenList4.countTokens(), 1ul);
    EXPECT_EQ(TokenList4.remainingString(), "maxBodySize");
    EXPECT_EQ(TokenList5.nextToken(), "maxBodySize");
    EXPECT_EQ(TokenList5.countTokens(), 0ul);
    EXPECT_EQ(TokenList5.remainingString(), "");
    EXPECT_EQ(TokenList6.nextToken(), "");
    EXPECT_EQ(TokenList6.countTokens(), 0ul);
    EXPECT_EQ(TokenList6.remainingString(), "");
}

TEST_F(StringTokenizerTestSuite, nextTokenOfSeparator) {
    EXPECT_EQ(TokenList1.nextToken(';'), "servername|listen|root|index|maxBodySize");
    EXPECT_EQ(TokenList1.countTokens(), 0ul);
    EXPECT_EQ(TokenList1.remainingString(), "");
    EXPECT_EQ(AdvancedTokenList1.nextToken('{'), "server");
    EXPECT_EQ(AdvancedTokenList1.countTokens(), 19ul);
    EXPECT_EQ(
        AdvancedTokenList1.remainingString(),
        "listen|80;|server_name|127.0.0.6;|root|www;|methods|GET;|index|index.html;|location|/"
        "images|{|methods|GET;|autoindex|on;|}|}"
    );
    EXPECT_EQ(
        AdvancedTokenList1.nextToken('}'),
        "listen|80;|server_name|127.0.0.6;|root|www;|methods|GET;|index|index.html;|location|/"
        "images|{|methods|GET;|autoindex|on;"
    );
    EXPECT_EQ(AdvancedTokenList1.countTokens(), 1ul);
    EXPECT_EQ(AdvancedTokenList1.remainingString(), "}");
    EXPECT_EQ(AdvancedTokenList2.nextToken(';'), "methods|GET");
    EXPECT_EQ(AdvancedTokenList2.countTokens(), 2ul);
    EXPECT_EQ(AdvancedTokenList2.remainingString(), "autoindex|on;");
}

TEST_F(StringTokenizerTestSuite, nextTokenOfSeparatorAtPos) {
    EXPECT_EQ(AdvancedTokenList1.nextToken(7ul), "server");
    EXPECT_EQ(
        AdvancedTokenList1.nextToken(AdvancedTokenList1.remainingString().size() - 1),
        "listen|80;|server_name|127.0.0.6;|root|www;|methods|GET;|index|index.html;|location|/"
        "images|{|methods|GET;|autoindex|on;|}"
    );
}
