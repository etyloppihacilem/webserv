#include "StringTokenizer.hpp"
#include "gtest/gtest.h"
#include <cstddef>
#include <string>
#include <tuple>

typedef std::string Name;
typedef std::string Input;
typedef char        Delim;
typedef std::string TokenizedString;
typedef std::string Token;
typedef std::size_t Count;

typedef std::tuple<Name, Input, Delim, TokenizedString> InitTest;

static const InitTest ListOfParam[]{
    { "OK", "----ffgsd skdf-dd-dd-/fsh--- - /df---", '-', "ffgsd skdf-dd-dd-/fsh- - /df" },
    { "OK_NoDelimInInput", "----ffgsd skdf-dd-dd-/fsh--- - /df---", 'x', "----ffgsd skdf-dd-dd-/fsh--- - /df---" },
    { "KO_EmptyInput", "", '-', "" },
};

class StringTokenizerParamInitTestFixture: public ::testing::TestWithParam<InitTest> {
    public:
        StringTokenizerParamInitTestFixture(): tokenizedTest(std::get<1>(GetParam()), std::get<2>(GetParam())) {}

        struct PrintToStringTestName {
                std::string operator()(const testing::TestParamInfo<InitTest> &info) const {
                    auto test_case = static_cast<InitTest>(info.param);
                    return std::get<0>(test_case);
                }
        };

    protected:
        StringTokenizer tokenizedTest;
};

TEST_P(StringTokenizerParamInitTestFixture, ObjectInstanciationTest) {
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
    StringTokenizerTests, StringTokenizerParamInitTestFixture, ::testing::ValuesIn(ListOfParam),
    StringTokenizerParamInitTestFixture::PrintToStringTestName()
);
