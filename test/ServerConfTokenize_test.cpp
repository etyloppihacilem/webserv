#include "ServerConfFields.hpp"
#include "ServerConfTokenize.hpp"
#include "StringTokenizer.hpp"
#include "gtest/gtest.h"
#include <string>

class ServerConfTokenizeTestSuite : public ::testing::Test {
    protected:
        ServerConfTokenizeTestSuite() {
        /*error.disable();*/
    }
        ~ServerConfTokenizeTestSuite() {
        /*error.enable();*/
    }
};

class ServerConfTokenizeFileTestSuite : public ServerConfTokenizeTestSuite {
    protected:
        ServerConfTokenizeFileTestSuite() :
            file_OK("http|{|server|{|toto|}|}"),
            file_KO_http("toto|{|server|{|toto|}|}"),
            file_KO_openBrace("http|server|{|toto|}|}"),
            file_KO_openBrace2("http{|server|{|toto|}|}"),
            file_KO_openBrace3("http|{server|{|toto|}|}"),
            file_KO_closeBrace("http|{|server|{|toto|}"),
            file_KO_closeBrace2("http|{|server|{|toto|}") {}

        std::string file_OK;
        std::string file_KO_http;
        std::string file_KO_openBrace;
        std::string file_KO_openBrace2;
        std::string file_KO_openBrace3;
        std::string file_KO_closeBrace;
        std::string file_KO_closeBrace2;
};

TEST_F(ServerConfTokenizeFileTestSuite, OK) {
    StringTokenizer result;
    ASSERT_NO_THROW(result = tokenizeFile(file_OK));
    EXPECT_EQ(result.remainingString(), "server|{|toto|}");
}

TEST_F(ServerConfTokenizeFileTestSuite, KO_wrongFieldName) {
    EXPECT_THROW(tokenizeFile(file_KO_http), ServerConfError);
}

TEST_F(ServerConfTokenizeFileTestSuite, KO_openBrace) {
    EXPECT_THROW(tokenizeFile(file_KO_openBrace), ServerConfError);
    EXPECT_THROW(tokenizeFile(file_KO_openBrace2), ServerConfError);
    EXPECT_THROW(tokenizeFile(file_KO_openBrace3), ServerConfError);
}

TEST_F(ServerConfTokenizeFileTestSuite, KO_closeBrace) {
    EXPECT_THROW(tokenizeFile(file_KO_closeBrace), ServerConfError);
    EXPECT_THROW(tokenizeFile(file_KO_closeBrace2), ServerConfError);
}

class ServerConfTokenizeServerTestSuite : public ServerConfTokenizeTestSuite {
    protected:
        ServerConfTokenizeServerTestSuite() :
            server_OK("server|{|server_name|localhost;|listen|80;|location|/|{|upload_path|/upload;|}|}", '|'),
            server_KO_server("titi|{|server_name|localhost;|listen|80;|location|/|{|upload_path|/upload;|}|}", '|'),
            server_KO_openBrace("server|server_name|localhost;|listen|80;|location|/|{|upload_path|/upload;|}|}", '|'),
            server_KO_openBrace2("server{|server_name|localhost;|listen|80;|location|/|{|upload_path|/upload;|}|}", '|'),
            server_KO_openBrace3("server|{server_name|localhost;|listen|80;|location|/|{|upload_path|/upload;|}|}", '|'),
            server_KO_closeBrace("server|{|server_name|localhost;|listen|80;|location|/|{|upload_path|/upload;|}", '|'),
            server_KO_closeBrace2("server|{|server_name|localhost;|listen|80;|location|/|{|upload_path|/upload;|error_page|404|}", '|') {}

        StringTokenizer server_OK;
        StringTokenizer server_KO_server;
        StringTokenizer server_KO_openBrace;
        StringTokenizer server_KO_openBrace2;
        StringTokenizer server_KO_openBrace3;
        StringTokenizer server_KO_closeBrace;
        StringTokenizer server_KO_closeBrace2;
};

TEST_F(ServerConfTokenizeServerTestSuite, OK_Suite) {
    StringTokenizer result;
    ASSERT_NO_THROW(result = tokenizeServer(server_OK));
    EXPECT_EQ(server_OK.remainingString(), "");
    EXPECT_EQ(result.remainingString(), "server_name|localhost;|listen|80;|location|/|{|upload_path|/upload;|}");
}

TEST_F(ServerConfTokenizeServerTestSuite, KO_wrongFieldName) {
    EXPECT_THROW(tokenizeServer(server_KO_server), ServerConfError);
}

TEST_F(ServerConfTokenizeServerTestSuite, KO_openBrace) {
    EXPECT_THROW(tokenizeServer(server_KO_openBrace), ServerConfError);
    EXPECT_THROW(tokenizeServer(server_KO_openBrace2), ServerConfError);
    EXPECT_THROW(tokenizeServer(server_KO_openBrace3), ServerConfError);
}

TEST_F(ServerConfTokenizeServerTestSuite, KO_closeBrace) {
    EXPECT_THROW(tokenizeServer(server_KO_closeBrace), ServerConfError);
    EXPECT_THROW(tokenizeServer(server_KO_closeBrace2), ServerConfError);
}

class ServerConfTokenizeLocationTestSuite : public ServerConfTokenizeTestSuite {
    protected:
        ServerConfTokenizeLocationTestSuite() :
            location_OK("location|/|{|upload_path|/upload;|error_page|404;|}", '|'),
            location_KO_location("tutu|/|{|upload_path|/upload;|error_page|404;|}", '|'),
            location_KO_openBrace("location|/|upload_path|/upload;|error_page|404;|}", '|'),
            location_KO_openBrace2("location|/{|upload_path|/upload;|error_page|404;|}", '|'),
            location_KO_openBrace3("location|/|{upload_path|/upload;|error_page|404;|}", '|'),
            location_KO_closeBrace("location|/|{|upload_path|/upload;|error_page|404;|", '|'),
            location_KO_closeBrace2("location|/|{|upload_path|/upload;|error_page|404;}", '|') {}

        StringTokenizer location_OK;
        StringTokenizer location_KO_location;
        StringTokenizer location_KO_openBrace;
        StringTokenizer location_KO_openBrace2;
        StringTokenizer location_KO_openBrace3;
        StringTokenizer location_KO_closeBrace;
        StringTokenizer location_KO_closeBrace2;
};

TEST_F(ServerConfTokenizeLocationTestSuite, OK_Suite) {
    Field result;
    ASSERT_NO_THROW(result = tokenizeLocation(location_OK));
    EXPECT_EQ(location_OK.remainingString(), "");
    EXPECT_EQ(result.first, "/");
    EXPECT_EQ(result.second.remainingString(), "upload_path|/upload;|error_page|404;");
}

TEST_F(ServerConfTokenizeLocationTestSuite, KO_wrongFieldName) {
    EXPECT_THROW(tokenizeLocation(location_KO_location), ServerConfError);
}

TEST_F(ServerConfTokenizeLocationTestSuite, KO_openBrace) {
    EXPECT_THROW(tokenizeLocation(location_KO_openBrace), ServerConfError);
    EXPECT_THROW(tokenizeLocation(location_KO_openBrace2), ServerConfError);
    EXPECT_THROW(tokenizeLocation(location_KO_openBrace3), ServerConfError);
}

TEST_F(ServerConfTokenizeLocationTestSuite, KO_closeBrace) {
    EXPECT_THROW(tokenizeLocation(location_KO_closeBrace), ServerConfError);
    EXPECT_THROW(tokenizeLocation(location_KO_closeBrace2), ServerConfError);
}

class ServerConfTokenizeFieldTestSuite : public ServerConfTokenizeTestSuite {
protected:
    ServerConfTokenizeFieldTestSuite() :
    field_OK("listen|8080;", '|'),
    field_OK_manyValues("index|index.html|index.php|index.py|index.js", '|'),
    field_KO_empty("autoindex", '|') {}

    StringTokenizer field_OK;
    StringTokenizer field_OK_manyValues;
    StringTokenizer field_KO_empty;
    // StringTokenizer field_KO_missingColonEndl;
};

TEST_F(ServerConfTokenizeFieldTestSuite, OK_Suite) {
    Field result;
    ASSERT_NO_THROW(result = tokenizeField(field_OK));
    EXPECT_EQ(field_OK.remainingString(), "");
    EXPECT_EQ(result.first, "listen");
    EXPECT_EQ(result.second.remainingString(), "8080");
    Field result2;
    ASSERT_NO_THROW(result2 = tokenizeField(field_OK_manyValues));
    EXPECT_EQ(field_OK_manyValues.remainingString(), "");
    EXPECT_EQ(result2.first, "index");
    EXPECT_EQ(result2.second.remainingString(), "index.html|index.php|index.py|index.js");
}

TEST_F(ServerConfTokenizeFieldTestSuite, KO_Suite) {
    EXPECT_THROW(tokenizeField(field_KO_empty), ServerConfWarn);
}
