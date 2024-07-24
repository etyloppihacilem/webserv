/* #############################################################################

               """          StringUtils_test.cpp
        -\-    _|__
         |\___/  . \        Created on 23 Jun. 2024 at 16:11
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "gtest/gtest.h"
#include <string>
#include "StringUtils.hpp"

TEST(StringUtilsTestSuite, extract_extension) {
    std::string a;
    a = "";
    EXPECT_EQ("", extract_extension(a));
    a = "coucou/je";
    EXPECT_EQ("", extract_extension(a));
    a = "coucou/je.suis";
    EXPECT_EQ("suis", extract_extension(a));
    a = "coucou.heureux/je.suis";
    EXPECT_EQ("suis", extract_extension(a));
    a = "je.suis";
    EXPECT_EQ("suis", extract_extension(a));
    a = ".coucou";
    EXPECT_EQ("", extract_extension(a));
    a = ".coucou.je";
    EXPECT_EQ("je", extract_extension(a));
    a = "eh/.coucou";
    EXPECT_EQ("", extract_extension(a));
    a = "eh/.coucou.je";
    EXPECT_EQ("je", extract_extension(a));
}

TEST(StringUtilsTestSuite, extract_basename) {
    std::string a;
    a = "";
    EXPECT_EQ("", extract_basename(a));
    a = "super";
    EXPECT_EQ("super", extract_basename(a));
    a = "/super";
    EXPECT_EQ("super", extract_basename(a));
    a = "/coucou/je/suis/heureux";
    EXPECT_EQ("heureux", extract_basename(a));
    a = "/coucou/je/suis/heureux.cpp";
    EXPECT_EQ("heureux.cpp", extract_basename(a));
    a = "/coucou/je/suis/heureux.cpp/";
    EXPECT_EQ("", extract_basename(a));
    a = "/";
    EXPECT_EQ("", extract_basename(a));
}
