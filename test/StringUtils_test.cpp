/* #############################################################################

               """          StringUtils_test.cpp
        -\-    _|__
         |\___/  . \        Created on 23 Jun. 2024 at 16:11
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "StringUtils.hpp"
#include "gtest/gtest.h"
#include <string>

TEST(StringUtilsTestSuite, add_trailing_slash) {
    std::string a;
    a = "";
    EXPECT_EQ("", add_trailing_slash(a));
    a = "/";
    EXPECT_EQ("/", add_trailing_slash(a));
    a = "coucou/je";
    EXPECT_EQ("coucou/je/", add_trailing_slash(a));
    a = "coucou/je/";
    EXPECT_EQ("coucou/je/", add_trailing_slash(a));
    a = "/recoucou/je/";
    EXPECT_EQ("/recoucou/je/", add_trailing_slash(a));
}

TEST(StringUtilsTestSuite, clean_trailing_slash) {
    std::string a;
    a = "";
    EXPECT_EQ("", clean_trailing_slash(a));
    a = "/";
    EXPECT_EQ("/", clean_trailing_slash(a));
    a = "coucou/je";
    EXPECT_EQ("coucou/je", clean_trailing_slash(a));
    a = "coucou/je/";
    EXPECT_EQ("coucou/je", clean_trailing_slash(a));
    a = "/recoucou/je/";
    EXPECT_EQ("/recoucou/je", clean_trailing_slash(a));
}

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

TEST(StringUtilsTestSuite, sanitize_HTTP_string) {
    EXPECT_EQ("Coucou je suis heureux", sanitize_HTTP_string("Coucou je suis heureux")) << "Nothing to change";
    EXPECT_EQ("Coucou je suis heureux", sanitize_HTTP_string("Coucou\rje\rsuis heureux")) << "Sanitizine lonely \\r";
    EXPECT_EQ("Coucou\nje\nsuis reux", sanitize_HTTP_string("Coucou\nje\nsuis reux")) << "Sanitizine lonely \\n";
    EXPECT_EQ(" Coucou je suis reux", sanitize_HTTP_string("\rCoucou\rje\rsuis reux")) << "Sanitizine lonely \\r BOL";
    EXPECT_EQ("Coucou je suis reux\r", sanitize_HTTP_string("Coucou\rje\rsuis reux\r")) << "Sanitizine lonely \\r EOL";
    EXPECT_EQ("ab\ncd", sanitize_HTTP_string("ab\r\ncd")) << "\\r\\n to \\n normal";
    // EXPECT_EQ("\nab\ncd", sanitize_HTTP_string("\r\nab\r\ncd")) << "\\r\\n to \\n BOL"; // not here
    EXPECT_EQ("ab\ncd\n", sanitize_HTTP_string("ab\r\ncd\r\n")) << "\\r\\n to \\n EOL";
    EXPECT_EQ("ab \n cd", sanitize_HTTP_string("ab\r\r\n\rcd")) << "\\r\\n to \\n limit";
}

TEST(StringUtilsTestSuite, sanitize_HTTP_stringLen) {
    std::string a = "ab\r\ncd\r\n";
    EXPECT_EQ("ab\ncd\r\n", sanitize_HTTP_string(a, 4));
}

TEST(StringUtilsTestSuite, sanitize_HTTP_stringEndOfHeader) {
    std::string a = "ab\r\n\r\ncd\r\nhihi";
    EXPECT_EQ("ab\n\ncd\r\nhihi", sanitize_HTTP_string(a, 0));
}

TEST(StringUtilsTestSuite, sanitize_HTTP_chunk) {
    std::string a = "super je suis heureux \r\nab\r\n\r\na\r\nCoucou je suis heureux";
    EXPECT_EQ("super je suis heureux \nab\n\na\r\nCoucou je suis heureux", sanitize_HTTP_string(a, 0));
}

TEST(StringUtilsTestSuite, sanitize_HTTP_stringEndOfHeader2) {
    std::string a = "\r\ncoucousuper\r\n";
    EXPECT_EQ("\ncoucousuper\r\n", sanitize_HTTP_string(a, 0));
}
