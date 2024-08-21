#include "ServerConfLogging.hpp"
#include "gtest/gtest.h"
#include <string>

TEST(ServerConfLoggingTestSuite, buildLoggingInfo) {
    std::string a;
    a = "";
    EXPECT_EQ("", buildLoggingInfo(a));
    a = "coucou|je";
    EXPECT_EQ("coucou je", buildLoggingInfo(a));
    a = "coucou je toto;";
    EXPECT_EQ("coucou je toto;", buildLoggingInfo(a));
    a = "coucou|je|suis|le|plus|beau!!! Qui suis je?";
    EXPECT_EQ("coucou je suis le plus beau!!!", buildLoggingInfo(a));
}
