#include "StringTokenizer.hpp"
#include "gtest/gtest.h"
#include <cstddef>
#include <string>
#include <tuple>

typedef std::string Name;
typedef std::string String;
typedef std::string Delim;
typedef std::size_t DelimSize;

struct test_tokenizer {
std::string name;
std::string tokenString;
std::string delimiter;
std::size_t delimSize;
};

class StringTokenizerParamTestFixture : public ::testing::TestWithParam<test_tokenizer> {
    public:
        StringTokenizerParamTestFixture():
            tokenizedTest("---sfdl-- toto- - 123/--", "-") {}

        struct PrintToStringParamName{
            template <class ParamType>
            std::string operator()(const testing::TestParamInfo<ParamType> &info) const{
                auto test_case = static_cast<test_tokenizer>(info.param);
                return test_case.name;
            }
        };

    protected:
        StringTokenizer tokenizedTest;
};

TEST_P(StringTokenizerParamTestFixture, ObjectInstanciationTest) {
    auto [name, string, delimiter, delimSize] = GetParam();
    ASSERT_EQ(  string, tokenizedTest.remainingString());
    ASSERT_EQ(  delimiter,  tokenizedTest.delimValue());
    ASSERT_EQ(  delimSize,  tokenizedTest.delimLen());
}

INSTANTIATE_TEST_SUITE_P(StringTokenizerTests, StringTokenizerParamTestFixture, ::testing::Values(
	test_tokenizer("IsValidObject", "sfdl- toto- - 123/", "-", 1)
	),
	StringTokenizerParamTestFixture::PrintToStringParamName());


// TEST(   StringTokeninzerTestSuite,  CountToken) {}
//
// TEST(   StringTokeninzerTestSuite,  PeakToken) {}
//
// TEST(   StringTokeninzerTestSuite,  nextToken) {}
//
// TEST(   StringTokeninzerTestSuite,  filterToken) {}
