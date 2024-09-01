#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "Route.hpp"
#include "Server.hpp"
#include "StringTokenizer.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include <map>
#include <set>
#include <string>
#include <vector>

class ServerTestSuite : public ::testing::Test {
    protected:
        ServerTestSuite() {
            warn.disable();
            info.disable();
            a = Server();
        }

        ~ServerTestSuite() {
            warn.enable();
            info.enable();
        }

        Server          a;
        StringTokenizer input;
};

TEST_F(ServerTestSuite, DefaultConstructor) {
    std::vector< std::string > expectedServer({ "localhost" });
    EXPECT_THAT(a.getServerName(), ::testing::ContainerEq(expectedServer));
    EXPECT_EQ(8080, a.getPort());
    EXPECT_EQ("www", a.getRootDir());
    EXPECT_EQ(1ul, a.getIndexPage().size());
    EXPECT_EQ("index.html", a.getIndexPage()[0]);
    EXPECT_EQ(false, a.getAutoindex());
    std::set< HttpMethod > expectedMethods({ GET });
    EXPECT_THAT(a.getMethods(), ::testing::ContainerEq(expectedMethods));
    EXPECT_EQ(1000000ul, a.getMaxBodySize());
    std::map< HttpCode, std::string > expectedError({});
    EXPECT_THAT(a.getErrorPages(), ::testing::ContainerEq(expectedError));
    EXPECT_TRUE(a.hasRoute("/"));
    EXPECT_EQ(false, a.hasServeNameSet());
    EXPECT_EQ(false, a.hasListenSet());
    EXPECT_EQ(false, a.hasRootSet());
    EXPECT_EQ(false, a.hasIndexPageSet());
    EXPECT_EQ(false, a.hasAutoindexSet());
    EXPECT_EQ(false, a.hasMethodsSet());
    EXPECT_EQ(false, a.hasMaxBodySizeSet());
}

TEST_F(ServerTestSuite, ParametrizeConstructorWithErrorPage) {
    input = StringTokenizer(
        "listen|8160;|root|www42;|index|test.html;|server_name|localhost|127.0.0.1;|client_max_body_size|1000;|methods|"
        "GET;|error_page|404|/error_pages/404.html;|error_page|500|/error_pages/500.html;|error_page|500|error_pages/"
        "redefinition.html;",
        '|'
    );
    a = Server(input);
    std::vector< std::string > expectedServer({ "localhost", "127.0.0.1" });
    EXPECT_THAT(a.getServerName(), ::testing::ContainerEq(expectedServer));
    EXPECT_EQ(8160, a.getPort());
    EXPECT_EQ("www42", a.getRootDir());
    std::vector< std::string > expectedIndex({ "test.html" });
    EXPECT_THAT(a.getIndexPage(), ::testing::ContainerEq(expectedIndex));
    EXPECT_EQ(false, a.getAutoindex());
    std::set< HttpMethod > expectedMethods({ GET });
    EXPECT_THAT(a.getMethods(), ::testing::ContainerEq(expectedMethods));
    EXPECT_EQ(1000ul, a.getMaxBodySize());
    std::map< HttpCode, std::string > expectedError({ { NotFound, "/error_pages/404.html" },
                                                      { InternalServerError, "/error_pages/500.html" } });
    EXPECT_THAT(a.getErrorPages(), ::testing::ContainerEq(expectedError));
    EXPECT_TRUE(a.hasRoute("/"));
    EXPECT_EQ(true, a.hasServeNameSet());
    EXPECT_EQ(true, a.hasListenSet());
    EXPECT_EQ(true, a.hasRootSet());
    EXPECT_EQ(true, a.hasIndexPageSet());
    EXPECT_EQ(false, a.hasAutoindexSet());
    EXPECT_EQ(true, a.hasMethodsSet());
    EXPECT_EQ(true, a.hasMaxBodySizeSet());
}

TEST_F(ServerTestSuite, ParametrizeConstructorServer1) {
    input = StringTokenizer(
        "listen|8080;|server_name|127.0.0.8;|root|www/hola;|methods|DELETE;|index|index.html;|autoindex|off;|", '|'
    );
    a = Server(input);
    std::vector< std::string > expectedServer({ "127.0.0.8" });
    EXPECT_THAT(a.getServerName(), ::testing::ContainerEq(expectedServer));
    EXPECT_EQ(8080, a.getPort());
    EXPECT_EQ("www/hola", a.getRootDir());
    std::vector< std::string > expectedIndex({ "index.html" });
    EXPECT_THAT(a.getIndexPage(), ::testing::ContainerEq(expectedIndex));
    EXPECT_EQ(false, a.getAutoindex());
    std::set< HttpMethod > expectedMethods({ DELETE });
    EXPECT_THAT(a.getMethods(), ::testing::ContainerEq(expectedMethods));
    EXPECT_EQ(1000000ul, a.getMaxBodySize());
    std::map< HttpCode, std::string > expectedError({});
    EXPECT_THAT(a.getErrorPages(), ::testing::ContainerEq(expectedError));
    EXPECT_TRUE(a.hasRoute("/"));
    EXPECT_EQ(true, a.hasServeNameSet());
    EXPECT_EQ(true, a.hasListenSet());
    EXPECT_EQ(true, a.hasRootSet());
    EXPECT_EQ(true, a.hasIndexPageSet());
    EXPECT_EQ(true, a.hasAutoindexSet());
    EXPECT_EQ(true, a.hasMethodsSet());
    EXPECT_EQ(false, a.hasMaxBodySizeSet());
}

TEST_F(ServerTestSuite, ParametrizeConstructorServer2) {
    input = StringTokenizer(
        "listen|8081;|server_name|127.0.0.3|127.0.0.4;|root|www;|methods|GET|POST;|index|index.html|star_citizen/"
        "home.html;|client_max_body_size|1000;|",
        '|'
    );
    a = Server(input);
    std::vector< std::string > expectedServer({ "127.0.0.3", "127.0.0.4" });
    EXPECT_THAT(a.getServerName(), ::testing::ContainerEq(expectedServer));
    EXPECT_EQ(8081, a.getPort());
    EXPECT_EQ("www", a.getRootDir());
    std::vector< std::string > expectedIndex({ "index.html", "star_citizen/home.html" });
    EXPECT_THAT(a.getIndexPage(), ::testing::ContainerEq(expectedIndex));
    EXPECT_EQ(false, a.getAutoindex());
    std::set< HttpMethod > expectedMethods({ GET, POST });
    EXPECT_THAT(a.getMethods(), ::testing::ContainerEq(expectedMethods));
    EXPECT_EQ(1000ul, a.getMaxBodySize());
    std::map< HttpCode, std::string > expectedError({});
    EXPECT_THAT(a.getErrorPages(), ::testing::ContainerEq(expectedError));
    EXPECT_TRUE(a.hasRoute("/"));
    EXPECT_EQ(true, a.hasServeNameSet());
    EXPECT_EQ(true, a.hasListenSet());
    EXPECT_EQ(true, a.hasRootSet());
    EXPECT_EQ(true, a.hasIndexPageSet());
    EXPECT_EQ(false, a.hasAutoindexSet());
    EXPECT_EQ(true, a.hasMethodsSet());
    EXPECT_EQ(true, a.hasMaxBodySizeSet());
}

TEST_F(ServerTestSuite, ParametrizeConstructorServerWithLocation) {
    input = StringTokenizer(
        "listen|8080;|server_name|127.0.0.6|127.0.0.7;|root|othersite;|methods|GET;|index|othersite.html;|location|/"
        "python|{|cgi_path|/usr/bin/python3;|root|cgi-bin;|file_ext|.py;|methods|GET|POST;|autoindex|off;|}|",
        '|'
    );
    a = Server(input);
    std::vector< std::string > expectedServer({ "127.0.0.6", "127.0.0.7" });
    EXPECT_THAT(a.getServerName(), ::testing::ContainerEq(expectedServer));
    EXPECT_EQ(8080, a.getPort());
    EXPECT_EQ("othersite", a.getRootDir());
    std::vector< std::string > expectedIndex({ "othersite.html" });
    EXPECT_THAT(a.getIndexPage(), ::testing::ContainerEq(expectedIndex));
    EXPECT_EQ(false, a.getAutoindex());
    std::set< HttpMethod > expectedMethods({ GET });
    EXPECT_THAT(a.getMethods(), ::testing::ContainerEq(expectedMethods));
    EXPECT_EQ(1000000ul, a.getMaxBodySize());
    std::map< HttpCode, std::string > expectedError({});
    EXPECT_THAT(a.getErrorPages(), ::testing::ContainerEq(expectedError));
    EXPECT_EQ(true, a.hasServeNameSet());
    EXPECT_EQ(true, a.hasListenSet());
    EXPECT_EQ(true, a.hasRootSet());
    EXPECT_EQ(true, a.hasIndexPageSet());
    EXPECT_EQ(false, a.hasAutoindexSet());
    EXPECT_EQ(true, a.hasMethodsSet());
    EXPECT_EQ(false, a.hasMaxBodySizeSet());
    EXPECT_TRUE(a.hasRoute("/"));
    ASSERT_TRUE(a.hasRoute("/python"));
    Route r = a.getRoute("/python");
    EXPECT_EQ("/usr/bin/python3", r.getCgiPath());
    EXPECT_EQ("cgi-bin/", r.getRootDir());
    EXPECT_EQ("py", r.getCgiExtension());
    expectedMethods = { GET, POST };
    EXPECT_THAT(r.getMethods(), ::testing::ContainerEq(expectedMethods));
    EXPECT_EQ(false, r.getAutoindex());
}
