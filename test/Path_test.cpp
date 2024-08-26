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

TEST_F(PathTestSuite, ConcatenationString) {
    path += "/";
    ASSERT_EQ(path.str(), "/");
    path += "/usr";
    ASSERT_EQ(path.str(), "/usr");
    path += "bin";
    ASSERT_EQ(path.str(), "/usr/bin");
    path += "sh/";
    ASSERT_EQ(path.str(), "/usr/bin/sh/");
    path += "/builtin/";
    ASSERT_EQ(path.str(), "/usr/bin/sh/builtin/");
}

extern bool filesystem_tests; // defined in main_test.cpp

#ifndef WORKDIR
# define WORKDIR ""
#endif

TEST_F(PathTestSuite, MakeAbsolute) {
    if (filesystem_tests)
        return; // not testing if no workdir
    std::string workdir = WORKDIR;
    ASSERT_EQ(Path("www/test").str(), std::string(WORKDIR "/www/test"));
}

TEST_F(PathTestSuite, EQ) {
    path = "/a/b/c/d/e/f";
    EXPECT_TRUE(path == Path("/a/b/c/d/e/f"));
    EXPECT_FALSE(path == Path("/a/b/c/"));
    EXPECT_FALSE(path == Path("/a/b/c/d/e/f/g"));
}

TEST_F(PathTestSuite, NE) {
    path = "/a/b/c/d/e/f";
    EXPECT_FALSE(path != Path("/a/b/c/d/e/f"));
    EXPECT_TRUE(path != Path("/a/b/c/"));
    EXPECT_TRUE(path != Path("/a/b/c/d/e/f/g"));
}

TEST_F(PathTestSuite, GT) {
    path = "/a/b/c/d/e/f";
    EXPECT_TRUE(path > Path("/a/b/c/d/e/f/g/h"));
    EXPECT_FALSE(path > Path("/a/b"));
    EXPECT_FALSE(path > Path("/a/b/c/d/e/f"));
}

TEST_F(PathTestSuite, LT) {
    path = "/a/b/c/d/e/f";
    EXPECT_FALSE(path < Path("/a/b/c/d/e/f/g/h"));
    EXPECT_TRUE(path < Path("/a/b"));
    EXPECT_FALSE(path < Path("/a/b/c/d/e/f"));
}

TEST_F(PathTestSuite, GE) {
    path = "/a/b/c/d/e/f";
    EXPECT_TRUE(path >= Path("/a/b/c/d/e/f/g/h"));
    EXPECT_FALSE(path >= Path("/a/b"));
    EXPECT_TRUE(path >= Path("/a/b/c/d/e/f"));
}

TEST_F(PathTestSuite, LE) {
    path = "/a/b/c/d/e/f";
    EXPECT_FALSE(path <= Path("/a/b/c/d/e/f/g/h"));
    EXPECT_TRUE(path <= Path("/a/b"));
    EXPECT_TRUE(path <= Path("/a/b/c/d/e/f"));
}

TEST_F(PathTestSuite, in) {
    path = "/a/b/c/d/e/f";
    EXPECT_TRUE(path.in(Path("/a/b/c/d")));
    EXPECT_FALSE(path.in(Path("/x/b/c/d")));
    EXPECT_FALSE(path.in(Path("/c/d")));
}
