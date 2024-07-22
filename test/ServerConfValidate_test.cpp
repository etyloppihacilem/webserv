#include "HttpMethods.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfValidate.hpp"
#include "gtest/gtest.h"
#include <cstddef>
#include <exception>
#include <string>

TEST(ServerConfValidateTestSuite, isValidConfFile) {
    EXPECT_TRUE(isValidConfFile("default.conf"));
    EXPECT_TRUE(isValidConfFile("config/default.conf"));
}

TEST(ServerConfValidateTestSuite, notValidConfFile) {
    EXPECT_FALSE(   isValidConfFile("default"));
    EXPECT_FALSE(   isValidConfFile("*****.conf"));
    EXPECT_FALSE(   isValidConfFile(".conf"));
    EXPECT_FALSE(   isValidConfFile("default.conff"));
    EXPECT_FALSE(   isValidConfFile("default.conf.conf"));
    EXPECT_FALSE(   isValidConfFile(""));
}

TEST(ServeConfValidateTestSuite, isValidFieldName) {
    for (int i = 0; i < COUNT_CONF_FIELD; ++i) {
        EXPECT_TRUE(isValidFieldName(ConfFieldString(i)));
    }
}

TEST(ServerConfValidateTestSuite, notValidFieldName) {
    EXPECT_FALSE(   isValidConfFile("toto"));
    EXPECT_FALSE(   isValidConfFile("https"));
    EXPECT_FALSE(   isValidConfFile(""));
}

TEST(ServerConfValidateTestSuite, isValidHostName) {
    EXPECT_TRUE(isValidHostname("localhost"));
    EXPECT_TRUE(isValidHostname("localhost.160"));
    EXPECT_TRUE(isValidHostname("vive-mon-JuJu"));
}

TEST(ServerConfValidateTestSuite, notValidHostName) {
    EXPECT_FALSE(isValidHostname("Juv_le_plus_beau"));
    EXPECT_FALSE(isValidHostname("-localhost"));
    EXPECT_FALSE(isValidHostname("localhost-"));
    EXPECT_FALSE(isValidHostname("local..host"));
    EXPECT_FALSE(isValidHostname("/42"));
	EXPECT_FALSE(isValidHostname("127.0.0.1"));
    EXPECT_FALSE(isValidHostname(""));
}

TEST(ServerConfValidateTestSuite, isValidIPAddress) {
	EXPECT_TRUE(isValidIPAddress("127.0.0.1"));
	EXPECT_TRUE(isValidIPAddress("160.0.0.160"));
}

TEST(ServerConfValidateTestSuite, notValidIPAddress) {
	EXPECT_FALSE(isValidIPAddress("localhost"));
	EXPECT_FALSE(isValidIPAddress("254,0.0.0"));
	EXPECT_FALSE(isValidIPAddress("254.0.0"));
	EXPECT_FALSE(isValidIPAddress("254.0.0.0.0"));
	EXPECT_FALSE(isValidIPAddress("2541.0.0.0"));
	EXPECT_FALSE(isValidIPAddress("255.0.0.0"));
	EXPECT_FALSE(isValidIPAddress("0.255.0.0"));
	EXPECT_FALSE(isValidIPAddress("0.0.255.0"));
EXPECT_FALSE(isValidIPAddress("0.0.0.255"));
    EXPECT_FALSE(isValidIPAddress(""));
}

TEST(ServerConfValidateTestSuite, isValidPort) {
	EXPECT_TRUE(isValidPort(8080));
}

TEST(ServerConfValidateTestSuite, notValidPort) {
	EXPECT_FALSE(isValidPort(80));
}



TEST(ServeConfValidateTestSuite, isValidMethods) {
    for (int i = 0; i < COUNT_METHOD; ++i) {
        EXPECT_TRUE(isValidMethods(method_string(i)));
    }
}

TEST(ServerConfValidateTestSuite, notValidMethods) {
    EXPECT_FALSE(   isValidMethods("GETS"));
    EXPECT_FALSE(   isValidMethods("get"));
    EXPECT_FALSE(   isValidMethods(""));
}

TEST(ServerConfValidateTestSuite, isValidPath) {
	EXPECT_TRUE(isValidUrl("/python"));
	EXPECT_TRUE(isValidUrl("/tetedelinotte/index.html"));
}

TEST(ServerConfValidateTestSuite, notValidPath) {
	EXPECT_FALSE(isValidUrl("python"));
	EXPECT_FALSE(isValidUrl("!@#$%^&*()+={}[]\\|;:\'\"?><,`~"));
	EXPECT_FALSE(isValidUrl(""));
}

TEST(ServerConfValidateTestSuite, isValidIndexFile) {
    EXPECT_TRUE(isValidIndexFile("index.html"));
}

TEST(ServerConfValidateTestSuite, notValidIndexFile) {
    EXPECT_FALSE(   isValidIndexFile("index"));
    EXPECT_FALSE(   isValidIndexFile("*****.html"));
    EXPECT_FALSE(   isValidIndexFile(".html"));
    EXPECT_FALSE(   isValidIndexFile("index.htmll"));
    EXPECT_FALSE(   isValidIndexFile("index.html.html"));
    EXPECT_FALSE(   isValidIndexFile(""));
}
