#include "ServerConfValidate.hpp"
#include "gtest/gtest.h"


TEST(ServerConfValidateTestSuite, isValidHostName) {
    EXPECT_TRUE(isValidHostname("localhost"));
    EXPECT_TRUE(isValidHostname("localhost.160"));
    EXPECT_TRUE(isValidHostname("vive-mon-JuJu"));
}

TEST(ServerConfValidateTestSuite, notValidHostName) {
    EXPECT_FALSE(isValidHostname(""));
    EXPECT_FALSE(isValidHostname("Juv_le_plus_beau"));
    EXPECT_FALSE(isValidHostname("-localhost"));
    EXPECT_FALSE(isValidHostname("localhost-"));
    EXPECT_FALSE(isValidHostname("local..host"));
    EXPECT_FALSE(isValidHostname("/42"));
    EXPECT_FALSE(isValidHostname("127.0.0.1"));
}

TEST(ServerConfValidateTestSuite, isValidIPAddress) {
    EXPECT_TRUE(isValidIPAddress("127.0.0.1"));
    EXPECT_TRUE(isValidIPAddress("160.0.0.160"));
}

TEST(ServerConfValidateTestSuite, notValidIPAddress) {
    EXPECT_FALSE(isValidIPAddress(""));
    EXPECT_FALSE(isValidIPAddress("localhost"));
    EXPECT_FALSE(isValidIPAddress("254,0.0.0"));
    EXPECT_FALSE(isValidIPAddress("254.0.0"));
    EXPECT_FALSE(isValidIPAddress("254.0.0.0.0"));
    EXPECT_FALSE(isValidIPAddress("2541.0.0.0"));
    EXPECT_FALSE(isValidIPAddress("355.0.0.0"));
    EXPECT_FALSE(isValidIPAddress("0.265.0.0"));
    EXPECT_FALSE(isValidIPAddress("0.0.285.0"));
    EXPECT_FALSE(isValidIPAddress("0.0.0.259"));
}

TEST(ServerConfValidateTestSuite, isValidPath) {
    EXPECT_TRUE(isValidPath("/python"));
    EXPECT_TRUE(isValidPath("python"));
    EXPECT_TRUE(isValidPath("/tetedelinotte/index.html"));
}

TEST(ServerConfValidateTestSuite, notValidPath) {
    EXPECT_FALSE(isValidPath(""));
    EXPECT_FALSE(isValidPath("!@#$%^&*()+={}[]\\|;:\'\"?><,`~"));
}

TEST(ServerConfValidateTestSuite, isValidExtension) {
    EXPECT_TRUE(isValidExtension(".py"));
    EXPECT_TRUE(isValidExtension(".cpp"));
    EXPECT_TRUE(isValidExtension(".c"));
}

TEST(ServerConfValidateTestSuite, notValidExtenstion) {
    EXPECT_FALSE(isValidExtension(""));
    EXPECT_FALSE(isValidExtension("file.py"));
    EXPECT_FALSE(isValidExtension("folder/file.py"));
    EXPECT_FALSE(isValidExtension("/folder/file.py"));
    EXPECT_FALSE(isValidExtension("..py"));
    EXPECT_FALSE(isValidExtension(".py.c"));
}
