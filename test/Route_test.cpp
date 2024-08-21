#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Route.hpp"
#include "Server.hpp"
#include "StringTokenizer.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include <set>
#include <string>
#include <vector>

class RouteTestSuite : public ::testing::Test {
    protected:
        RouteTestSuite() { /*error.disable();*/ a = Route(server); }

        ~RouteTestSuite() { /*error.enable();*/ }

        Route           a;
        Server          server;
        StringTokenizer input;
};

TEST_F(RouteTestSuite, DefaultConstructor) {
    EXPECT_EQ("/", a.getLocation());
    EXPECT_EQ("www/", a.getRootDir());
    std::vector< std::string > expectedIndex({ "index.html" });
    EXPECT_THAT(a.getIndexPage(), ::testing::ContainerEq(expectedIndex));
    EXPECT_EQ(true, a.getAutoindex());
    std::set< HttpMethod > expected({ GET });
    EXPECT_THAT(a.getMethods(), ::testing::ContainerEq(expected));
    EXPECT_EQ("www/", a.getUploadPath());
    EXPECT_EQ(OK, a.getRedirCode());
    EXPECT_EQ("", a.getRedirPage());
    EXPECT_EQ("", a.getCgiPath());
    EXPECT_EQ("", a.getCgiExtension());
    EXPECT_EQ(false, a.hasRootSet());
    EXPECT_EQ(false, a.hasIndexPageSet());
    EXPECT_EQ(false, a.hasAutoindexSet());
    EXPECT_EQ(false, a.hasMethodsSet());
    EXPECT_EQ(false, a.hasUploadSet());
    EXPECT_EQ(false, a.hasRedirSet());
    EXPECT_EQ(false, a.hasCgiPathSet());
    EXPECT_EQ(false, a.hasCgiExtensionSet());
}

TEST_F(RouteTestSuite, ParametrizeConstructorValidBasic) {
    input = StringTokenizer("methods|GET;|autoindex|off;", '|');
    a     = Route("/images", input, server);
    EXPECT_EQ("/images", a.getLocation());
    EXPECT_EQ("www/images/", a.getRootDir());
    std::vector< std::string > expectedIndex({ "index.html" });
    EXPECT_THAT(a.getIndexPage(), ::testing::ContainerEq(expectedIndex));
    EXPECT_EQ(false, a.getAutoindex());
    std::set< HttpMethod > expected({ GET });
    EXPECT_THAT(a.getMethods(), ::testing::ContainerEq(expected));
    EXPECT_EQ("www/images/", a.getUploadPath());
    EXPECT_EQ(OK, a.getRedirCode());
    EXPECT_EQ("", a.getRedirPage());
    EXPECT_EQ("", a.getCgiPath());
    EXPECT_EQ("", a.getCgiExtension());
    EXPECT_EQ(false, a.hasRootSet());
    EXPECT_EQ(false, a.hasIndexPageSet());
    EXPECT_EQ(true, a.hasAutoindexSet());
    EXPECT_EQ(true, a.hasMethodsSet());
    EXPECT_EQ(false, a.hasUploadSet());
    EXPECT_EQ(false, a.hasRedirSet());
    EXPECT_EQ(false, a.hasCgiPathSet());
    EXPECT_EQ(false, a.hasCgiExtensionSet());
}

TEST_F(RouteTestSuite, ParametrizeConstructorValidUpload) {
    input = StringTokenizer("methods|GET|POST;|index|index.html;|autoindex|on;|upload_path|/uploads;", '|');
    a     = Route("/", input, server);
    EXPECT_EQ("/", a.getLocation());
    EXPECT_EQ("www/", a.getRootDir());
    std::vector< std::string > expectedIndex({ "index.html" });
    EXPECT_THAT(a.getIndexPage(), ::testing::ContainerEq(expectedIndex));
    EXPECT_EQ(true, a.getAutoindex());
    std::set< HttpMethod > expected({ GET, POST });
    EXPECT_THAT(a.getMethods(), ::testing::ContainerEq(expected));
    EXPECT_EQ("/uploads/", a.getUploadPath());
    EXPECT_EQ(OK, a.getRedirCode());
    EXPECT_EQ("", a.getRedirPage());
    EXPECT_EQ("", a.getCgiPath());
    EXPECT_EQ("", a.getCgiExtension());
    EXPECT_EQ(false, a.hasRootSet());
    EXPECT_EQ(true, a.hasIndexPageSet());
    EXPECT_EQ(true, a.hasAutoindexSet());
    EXPECT_EQ(true, a.hasMethodsSet());
    EXPECT_EQ(true, a.hasUploadSet());
    EXPECT_EQ(false, a.hasRedirSet());
    EXPECT_EQ(false, a.hasCgiPathSet());
    EXPECT_EQ(false, a.hasCgiExtensionSet());
}

TEST_F(RouteTestSuite, ParametrizeConstructorValidRewrite) {
    input = StringTokenizer("methods|GET;|autoindex|off;|rewrite|301|/redir/newLocation.html;", '|');
    a     = Route("/YouHaveBeenRedirect", input, server);
    EXPECT_EQ("/YouHaveBeenRedirect", a.getLocation());
    EXPECT_EQ("www/YouHaveBeenRedirect/", a.getRootDir());
    std::vector< std::string > expectedIndex({ "index.html" });
    EXPECT_THAT(a.getIndexPage(), ::testing::ContainerEq(expectedIndex));
    EXPECT_EQ(false, a.getAutoindex());
    std::set< HttpMethod > expected({ GET });
    EXPECT_THAT(a.getMethods(), ::testing::ContainerEq(expected));
    EXPECT_EQ("www/YouHaveBeenRedirect/", a.getUploadPath());
    EXPECT_EQ(MovedPermanently, a.getRedirCode());
    EXPECT_EQ("/redir/newLocation.html", a.getRedirPage());
    EXPECT_EQ("", a.getCgiPath());
    EXPECT_EQ("", a.getCgiExtension());
    EXPECT_EQ(false, a.hasRootSet());
    EXPECT_EQ(false, a.hasIndexPageSet());
    EXPECT_EQ(true, a.hasAutoindexSet());
    EXPECT_EQ(true, a.hasMethodsSet());
    EXPECT_EQ(false, a.hasUploadSet());
    EXPECT_EQ(true, a.hasRedirSet());
    EXPECT_EQ(false, a.hasCgiPathSet());
    EXPECT_EQ(false, a.hasCgiExtensionSet());
}

TEST_F(RouteTestSuite, ParametrizeConstructorValidCgi) {
    input = StringTokenizer(
        "methods|GET|POST;|autoindex|off;|root|cgi-bin;|file_ext|.py;|cgi_path|/usr/bin/python3;", '|'
    );
    a = Route("/Python", input, server);
    EXPECT_EQ("/Python", a.getLocation());
    EXPECT_EQ("cgi-bin/", a.getRootDir());
    std::vector< std::string > expectedIndex({ "index.html" });
    EXPECT_THAT(a.getIndexPage(), ::testing::ContainerEq(expectedIndex));
    EXPECT_EQ(false, a.getAutoindex());
    std::set< HttpMethod > expected({ GET, POST });
    EXPECT_THAT(a.getMethods(), ::testing::ContainerEq(expected));
    EXPECT_EQ("cgi-bin/", a.getUploadPath());
    EXPECT_EQ(OK, a.getRedirCode());
    EXPECT_EQ("", a.getRedirPage());
    EXPECT_EQ("/usr/bin/python3/", a.getCgiPath());
    EXPECT_EQ(".py", a.getCgiExtension());
    EXPECT_EQ(true, a.hasRootSet());
    EXPECT_EQ(false, a.hasIndexPageSet());
    EXPECT_EQ(true, a.hasAutoindexSet());
    EXPECT_EQ(true, a.hasMethodsSet());
    EXPECT_EQ(false, a.hasUploadSet());
    EXPECT_EQ(false, a.hasRedirSet());
    EXPECT_EQ(true, a.hasCgiPathSet());
    EXPECT_EQ(true, a.hasCgiExtensionSet());
}

TEST_F(RouteTestSuite, ParametrizeConstructorOnlyInvalid) {
    input = StringTokenizer(
        "root|/oupsie?;|index|toto;|methods|PUT;|autoindex|true;|upload_path|toto.hmtl;|rewrite|200|/redir/"
        "toto.html;|cgi_path|90()kfj;|file_ext|titi;",
        '|'
    );
    a = Route("/", input, server);
    EXPECT_EQ("/", a.getLocation());
    EXPECT_EQ("www/", a.getRootDir());
    std::vector< std::string > expectedIndex({ "index.html" });
    EXPECT_THAT(a.getIndexPage(), ::testing::ContainerEq(expectedIndex));
    EXPECT_EQ(true, a.getAutoindex());
    std::set< HttpMethod > expected({ GET });
    EXPECT_THAT(a.getMethods(), ::testing::ContainerEq(expected));
    EXPECT_EQ("www/", a.getUploadPath());
    EXPECT_EQ(OK, a.getRedirCode());
    EXPECT_EQ("", a.getRedirPage());
    EXPECT_EQ("", a.getCgiPath());
    EXPECT_EQ("", a.getCgiExtension());
    EXPECT_EQ(false, a.hasRootSet());
    EXPECT_EQ(false, a.hasIndexPageSet());
    EXPECT_EQ(false, a.hasAutoindexSet());
    EXPECT_EQ(false, a.hasMethodsSet());
    EXPECT_EQ(false, a.hasUploadSet());
    EXPECT_EQ(false, a.hasRedirSet());
    EXPECT_EQ(false, a.hasCgiPathSet());
    EXPECT_EQ(false, a.hasCgiExtensionSet());
}
