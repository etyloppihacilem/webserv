#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfSetter.hpp"
#include "gtest/gtest.h"
#include <climits>
#include <gmock/gmock.h>
#include <set>
#include <string>
#include <vector>

class ServerConfSetterTestSuite : public ::testing::Test {
    protected:
        ServerConfSetterTestSuite() { /*error.disable();*/ }

        ~ServerConfSetterTestSuite() { /*error.enable();*/ }
};

// SetRootDir_Begin
TEST_F(ServerConfSetterTestSuite, SetRootDir_wrongValuesCount) {
    ASSERT_THROW(setFieldRoot({}), ServerConfWarn);
    ASSERT_THROW(setFieldRoot({ "wwwc", "tooMany" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetRootDir_isNotValidRootDirValue) {
    ASSERT_THROW(setFieldRoot({ "" }), ServerConfWarn);
    ASSERT_THROW(setFieldRoot({ "/www" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetRootDir_isValidRootDirValue) {
    ASSERT_EQ(setFieldRoot({ "www42" }), "www42/");
    ASSERT_EQ(setFieldRoot({ "www/images/" }), "www/images/");
}

// SetRootDir_End

// SetFieldIndex_Begin
TEST_F(ServerConfSetterTestSuite, setFieldIndex_isNotValidValues) {
    ASSERT_THROW(setFieldIndex({}), ServerConfWarn);
    ASSERT_THROW(setFieldIndex({ "", "toto", ".html" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldIndex_isValidValues) {
    std::vector< std::string > expected = { "index.html", "index.php", "index.py" };
    std::vector< std::string > result;
    ASSERT_NO_THROW(result = setFieldIndex({ "index.html", "index.php", "index.py" }));
    ASSERT_THAT(result, testing::ContainerEq(expected));
}

TEST_F(ServerConfSetterTestSuite, SetFieldIndex_mixValidAndWrongValues) {
    std::vector< std::string > expected = { "index.html", "index.py" };
    std::vector< std::string > result;
    ASSERT_NO_THROW(result = setFieldIndex({ "index.html", "toto", "index.py" }));
    EXPECT_EQ(result.size(), expected.size());
    EXPECT_THAT(result, testing::ContainerEq(expected));
}

// SetFieldIndex_End

// SetFieldAutoindex_Begin
TEST_F(ServerConfSetterTestSuite, SetFieldAutoindex_wrongValueCount) {
    EXPECT_THROW(setFieldAutoindex({}), ServerConfWarn);
    EXPECT_THROW(setFieldAutoindex({ "on", "tooMany" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldAutoIndex_isNotValidValue) {
    EXPECT_THROW(setFieldAutoindex({ "" }), ServerConfWarn);
    EXPECT_THROW(setFieldAutoindex({ "0" }), ServerConfWarn);
    EXPECT_THROW(setFieldAutoindex({ "1" }), ServerConfWarn);
    EXPECT_THROW(setFieldAutoindex({ "On" }), ServerConfWarn);
    EXPECT_THROW(setFieldAutoindex({ "Off" }), ServerConfWarn);
    EXPECT_THROW(setFieldAutoindex({ "toto" }), ServerConfWarn);
    EXPECT_THROW(setFieldAutoindex({ "onn" }), ServerConfWarn);
    EXPECT_THROW(setFieldAutoindex({ "offf" }), ServerConfWarn);
    EXPECT_THROW(setFieldAutoindex({ "oon" }), ServerConfWarn);
    EXPECT_THROW(setFieldAutoindex({ "ooff" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldAutoIndex_isValidValue) {
    bool result;
    ASSERT_NO_THROW(result = setFieldAutoindex({ "off" }));
    EXPECT_EQ(result, false);
    bool result2;
    ASSERT_NO_THROW(result2 = setFieldAutoindex({ "on" }));
    EXPECT_EQ(result2, true);
}

// SetFieldAutoindex_End

// SetFieldMethods_Begin
TEST_F(ServerConfSetterTestSuite, SetFieldMethods_isNotValidValues) {
    EXPECT_THROW(setFieldMethods({}), ServerConfWarn);
    EXPECT_THROW(setFieldMethods({ "" }), ServerConfWarn);
    EXPECT_THROW(setFieldMethods({ "get" }), ServerConfWarn);
    EXPECT_THROW(setFieldMethods({ "post" }), ServerConfWarn);
    EXPECT_THROW(setFieldMethods({ "delete" }), ServerConfWarn);
    EXPECT_THROW(setFieldMethods({ "GETT" }), ServerConfWarn);
    EXPECT_THROW(setFieldMethods({ "GGET" }), ServerConfWarn);
    EXPECT_THROW(setFieldMethods({ "GET GET" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldMethods_isValidValues) {
    std::set< HttpMethod > expected = { GET, POST, DELETE };
    std::set< HttpMethod > result;
    ASSERT_NO_THROW(result = setFieldMethods({ "GET", "POST", "DELETE" }));
    EXPECT_EQ(result.size(), expected.size());
}

TEST_F(ServerConfSetterTestSuite, SetFieldMethods_mixValidAndWrongValues) {
    std::set< HttpMethod > expected = { POST, DELETE };
    std::set< HttpMethod > result;
    ASSERT_NO_THROW(result = setFieldMethods({ "toto", "POST", "DELETE", "" }));
    EXPECT_EQ(result.size(), expected.size());
}

// SetFieldAutoindex_End

// SetFieldAutoindex_Begin
TEST_F(ServerConfSetterTestSuite, SetFieldMaxBodySize_wrongValueCount) {
    EXPECT_THROW(setFieldMaxBodySize({}), ServerConfWarn);
    EXPECT_THROW(setFieldMaxBodySize({ "10000", "tooMany" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldMaxBodySize_isNotValidIntValue) {
    EXPECT_THROW(setFieldErrorPageCode({ "" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPageCode({ "toto" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPageCode({ "123456789011" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPageCode({ "-12345678901" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldMaxBodySize_isNotValidValue) {
    EXPECT_THROW(setFieldMaxBodySize({ "0" }), ServerConfWarn);
    EXPECT_THROW(setFieldMaxBodySize({ "-123" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldMaxBodySize_isValidValue) {
    EXPECT_EQ(setFieldMaxBodySize({ "1000000" }), 1000000);
    EXPECT_EQ(setFieldMaxBodySize({ "2147483647" }), INT_MAX);
}

// SetFieldMaxBodySize_End

// SetFieldErrorPageCode_Begin
TEST_F(ServerConfSetterTestSuite, SetFieldErrorPageCode_wrongValueCount) {
    EXPECT_THROW(setFieldErrorPageCode({}), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPageCode({ "404", "NotFound.html", "tooMany" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldErrorPageCode_isNotValidIntValue) {
    EXPECT_THROW(setFieldErrorPageCode({ "", "toto" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPageCode({ "toto", "toto" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPageCode({ "123456789011", "toto" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPageCode({ "-12345678901", "toto" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldErrorPageCode_isNotValidErrorCode) {
    EXPECT_THROW(setFieldErrorPageCode({ "0", "toto" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPageCode({ "-123", "toto" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPageCode({ "100", "/Error/Continue.html" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPageCode({ "200", "/Error/OK.html" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldErrorPageCode_isValidErrorCode) {
    EXPECT_EQ(setFieldErrorPageCode({ "404", "/Error/NotFound.html" }), NotFound);
    EXPECT_EQ(setFieldErrorPageCode({ "500", "/Error/InternalServerError.html" }), InternalServerError);
    EXPECT_EQ(setFieldErrorPageCode({ "900", "/Error/RandomError.html" }), 900);
}

// SetFieldErrorPageCode_End

// SetFieldErrorPagePath_Begin
TEST_F(ServerConfSetterTestSuite, SetFieldErrorPagePath_wrongValueCount) {
    EXPECT_THROW(setFieldErrorPagePath({}), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPagePath({ "404", "NotFound.html", "tooMany" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldErrorPagePath_isNotValidFilePath) {
    EXPECT_THROW(setFieldErrorPagePath({ "" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPagePath({ "toto.html" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPagePath({ "200", "Error/Continue" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPagePath({ "200", "/Error/Continue" }), ServerConfWarn);
    EXPECT_THROW(setFieldErrorPagePath({ "200", "/Error/.html" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldErrorPagePath_isValidFilePath) {
    EXPECT_EQ(setFieldErrorPagePath({ "404", "/Error/NotFound.html" }), "/Error/NotFound.html");
    EXPECT_EQ(setFieldErrorPagePath({ "500", "/Error/InternalServerError.html" }), "/Error/InternalServerError.html");
    EXPECT_EQ(setFieldErrorPagePath({ "900", "/Error/RandomError.html" }), "/Error/RandomError.html");
}

// SetFieldErrorPagePath_End

// SetFieldLocation_Begin
TEST_F(ServerConfSetterTestSuite, SetFieldLocationPath_isNotValidFolderPath) {
    EXPECT_THROW(setFieldLocationPath(""), ServerConfWarn);
    EXPECT_THROW(setFieldLocationPath("images"), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldLocationPath_isValidFolderPath) {
    EXPECT_EQ(setFieldLocationPath("/"), "/");
    EXPECT_EQ(setFieldLocationPath("/images"), "/images");
    EXPECT_EQ(setFieldLocationPath("/images/upload"), "/images/upload");
}

// SetFieldLocation_End

// SetFieldUploadPath_Begin
TEST_F(ServerConfSetterTestSuite, SetFieldUploadPath_wrongValueCount) {
    EXPECT_THROW(setFieldUploadPath({}), ServerConfWarn);
    EXPECT_THROW(setFieldUploadPath({ "/images", "tooMany" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldUploadPath_isNotValidFolderPath) {
    EXPECT_THROW(setFieldUploadPath({ "" }), ServerConfWarn);
    EXPECT_THROW(setFieldUploadPath({ "images" }), ServerConfWarn);
    EXPECT_THROW(setFieldUploadPath({ "/images/upload.html" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldUploadPath_isValidFolderPath) {
    EXPECT_EQ(setFieldUploadPath({ "/" }), "/");
    EXPECT_EQ(setFieldUploadPath({ "/images" }), "/images/");
    EXPECT_EQ(setFieldUploadPath({ "/images/upload" }), "/images/upload/");
    EXPECT_EQ(setFieldUploadPath({ "/images/upload/" }), "/images/upload/");
}

// SetFieldUploadPath_End

// SetFieldRewriteCode_Begin
TEST_F(ServerConfSetterTestSuite, SetFieldRewriteCode_wrongValueCount) {
    EXPECT_THROW(setFieldRewriteCode({}), ServerConfWarn);
    EXPECT_THROW(setFieldRewriteCode({ "301", "NotFound.html", "tooMany" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldRewriteCode_isNotValidInt) {
    EXPECT_THROW(setFieldRewriteCode({ "", "toto" }), ServerConfWarn);
    EXPECT_THROW(setFieldRewriteCode({ "toto", "toto" }), ServerConfWarn);
    EXPECT_THROW(setFieldRewriteCode({ "123456789011", "toto" }), ServerConfWarn);
    EXPECT_THROW(setFieldRewriteCode({ "-12383479659", "toto" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldRewriteCode_isNotValidRedirCode) {
    EXPECT_THROW(setFieldRewriteCode({ "0", "toto" }), ServerConfWarn);
    EXPECT_THROW(setFieldRewriteCode({ "-123", "toto" }), ServerConfWarn);
    EXPECT_THROW(setFieldRewriteCode({ "100", "/Redir/Continue.html" }), ServerConfWarn);
    EXPECT_THROW(setFieldRewriteCode({ "200", "/Redir/OK.html" }), ServerConfWarn);
    EXPECT_THROW(setFieldRewriteCode({ "404", "/Redir/NotFound.html" }), ServerConfWarn);
    EXPECT_THROW(setFieldRewriteCode({ "500", "/Redir/InternalServerError.html" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldRewriteCode_isValidRedirCode) {
    EXPECT_EQ(setFieldRewriteCode({ "301", "/Redir/MovedPermanently.html" }), MovedPermanently);
    EXPECT_EQ(setFieldRewriteCode({ "310", "/Redir/InternalServerError.html" }), TemporaryRedirect);
    EXPECT_EQ(setFieldRewriteCode({ "306", "/Redir/RandomError.html" }), TemporaryRedirect);
    EXPECT_EQ(setFieldRewriteCode({ "309", "/Redir/RandomError.html" }), TemporaryRedirect);
}

// SetFieldRewriteCode_End

// SetFieldCgiPath_Begin
TEST_F(ServerConfSetterTestSuite, SetFieldCgiPath_wrongValueCount) {
    EXPECT_THROW(setFieldCgiPath({}), ServerConfWarn);
    EXPECT_THROW(setFieldCgiPath({ "/images", "tooMany" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldCgiPath_isNotValidFolderPath) {
    EXPECT_THROW(setFieldCgiPath({ "" }), ServerConfWarn);
    EXPECT_THROW(setFieldCgiPath({ "images" }), ServerConfWarn);
    EXPECT_THROW(setFieldCgiPath({ "/images/cgi.html" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldCgiPath_isValidFolderPath) {
    EXPECT_EQ(setFieldCgiPath({ "/" }), "/");
    EXPECT_EQ(setFieldCgiPath({ "/images" }), "/images/");
    EXPECT_EQ(setFieldCgiPath({ "/images/cgi" }), "/images/cgi/");
    EXPECT_EQ(setFieldCgiPath({ "/images/cgi/" }), "/images/cgi/");
}

// SetFieldCgiPath_End

// SetFieldFileExt_Begin
TEST_F(ServerConfSetterTestSuite, SetFieldFileExt_wrongValueCount) {
    EXPECT_THROW(setFieldFileExt({}), ServerConfWarn);
    EXPECT_THROW(setFieldFileExt({ ".html", ".php" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldFileExt_isNotValidExtension) {
    EXPECT_THROW(setFieldFileExt({ "" }), ServerConfWarn);
    EXPECT_THROW(setFieldFileExt({ "html" }), ServerConfWarn);
    EXPECT_THROW(setFieldFileExt({ "..html" }), ServerConfWarn);
    EXPECT_THROW(setFieldFileExt({ "/index/index.html" }), ServerConfWarn);
    EXPECT_THROW(setFieldFileExt({ "/index/index.html" }), ServerConfWarn);
}

TEST_F(ServerConfSetterTestSuite, SetFieldFileExt_isValidExtension) {
    EXPECT_EQ(setFieldFileExt({ ".html" }), ".html");
    EXPECT_EQ(setFieldFileExt({ ".php" }), ".php");
}

// SetFieldFileExt_End
