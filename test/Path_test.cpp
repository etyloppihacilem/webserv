#include "Path.hpp"
#include "gtest/gtest.h"
#include <string>

class PathTestSuite : public ::testing::Test {
    protected:
        PathTestSuite() : path(), str() {}

        Path        path;
        std::string str;
};

TEST_F(PathTestSuite, DefaultConstructor) {
    EXPECT_EQ(path.str(), "");
}

TEST_F(PathTestSuite, ParametrizeConstructor) {
    Path toto("/usr/bin/sh");
    EXPECT_EQ(toto.str(), "/usr/bin/sh");
}

TEST_F(PathTestSuite, AssignationPath) {
    Path toto("/usr/bin/sh");
    path = toto;
    EXPECT_EQ(path.str(), toto.str());
}

TEST_F(PathTestSuite, AssignationString) {
    path = "/usr/bin/python3";
    EXPECT_EQ(path.str(), "/usr/bin/python3");
}

TEST_F(PathTestSuite, ConcatenationPath) {
    path += Path("/");
    ASSERT_EQ(path.str(), "/");
    path += Path("/usr");
    ASSERT_EQ(path.str(), "/usr");
    path += Path("bin");
    ASSERT_EQ(path.str(), "/usr/bin");
    path += Path("sh/");
    ASSERT_EQ(path.str(), "/usr/bin/sh/");
    path += Path("/builtin/");
    ASSERT_EQ(path.str(), "/usr/bin/sh/builtin/");
}

TEST_F(PathTestSuite, ConcatenationString) {
}
