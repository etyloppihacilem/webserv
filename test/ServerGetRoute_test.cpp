/* #####################################################################################################################

               """          ServerGetRoute_test.cpp
        -\-    _|__
         |\___/  . \        Created on 21 Aug. 2024 at 17:24
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "Logger.hpp"
#include "Route.hpp"
#include "Server.hpp"
#include "ServerConfFields.hpp"
#include "ServerGetRoute.hpp"
#include "StringTokenizer.hpp"
#include "gtest/gtest.h"

// TEST_P(ResponseBuildStateFixture, speed_test) {
//     for (size_t i = 0; i < 100000; i++) {
//         _server.getRoute("/images/png/delete/this/image/is/really/long.png");
//         _server.getRoute("/coucou.html");
//         _server.getRoute("/space_corridor/search/ici?ici=coucou");
//         _server.getRoute("/test");
//         _server.getRoute("/");
//     }
// }

//
// TEST_P(ResponseBuildStateFixture, speed_test2) {
//     for (size_t i = 0; i < 100000; i++) {
//         _server.getRoute2("/images/png/delete/this/image/is/really/long.png");
//         _server.getRoute2("/coucou.html");
//         _server.getRoute2("/space_corridor/search/ici?ici=coucou");
//         _server.getRoute2("/test");
//         _server.getRoute2("/");
//     }
// }

class ServerRouteTestSuite : public ::testing::Test {
    protected:
        ServerRouteTestSuite() {
            info.disable();
            location = Field("/python", StringTokenizer("methods|GET;", '|'));
            a.addRoute(location);
            location = Field("/empty", StringTokenizer("", '|'));
            a.addRoute(location);
            location = Field("/star_trek/redir/to/star_citizen", StringTokenizer("", '|'));
            a.addRoute(location);
            location = Field("/test", StringTokenizer("upload_path|/upload;|file_ext|.bla;", '|'));
            a.addRoute(location);
            location = Field("/test/diff", StringTokenizer("upload_path|/upload_diff;", '|'));
            a.addRoute(location);
            location = Field("/test/diff/deepdiff", StringTokenizer("upload_path|/upload_diff;|file_ext|.diff;", '|'));
            a.addRoute(location);
            location = Field("/space_travel", StringTokenizer("", '|'));
            a.addRoute(location);
        }

        ~ServerRouteTestSuite() { info.enable(); }

        Server a;
        Field  location;
};

TEST_F(ServerRouteTestSuite, hasRoute_KO) {
    EXPECT_FALSE(a.hasRoute(""));
    EXPECT_FALSE(a.hasRoute("/python/"));
    EXPECT_FALSE(a.hasRoute("/star_trek"));
    EXPECT_FALSE(a.hasRoute("/star_trek/redir/to"));
    EXPECT_FALSE(a.hasRoute("/startrek/redir/to"));
}

TEST_F(ServerRouteTestSuite, hasRoute_OK) {
    EXPECT_TRUE(a.hasRoute("/"));
    EXPECT_TRUE(a.hasRoute("/python"));
    EXPECT_TRUE(a.hasRoute("/empty"));
    EXPECT_TRUE(a.hasRoute("/star_trek/redir/to/star_citizen"));
}

TEST_F(ServerRouteTestSuite, getRoute_KO) {
    EXPECT_EQ(a.getRoute("/nothere").getLocation(), "/");
    EXPECT_EQ(a.getRoute("/emptyy").getLocation(), "/");
    EXPECT_EQ(a.getRoute("/space_travel.d").getLocation(), "/");
    EXPECT_EQ(a.getRoute("/*/diff").getLocation(), "/");
}

TEST_F(ServerRouteTestSuite, getRoute_OK) {
    EXPECT_EQ(a.getRoute("/test/upload.txt").getLocation(), "/test");
    EXPECT_EQ(a.getRoute("/test/delete.png").getLocation(), "/test");
    EXPECT_EQ(a.getRoute("/test/diff/delete.png").getLocation(), "/test/diff");
    EXPECT_EQ(a.getRoute("/space_travel/some/thing").getLocation(), "/space_travel");
    EXPECT_EQ(a.getRoute("/space_travel/search/ici?ici=coucou").getLocation(), "/space_travel");
}

TEST_F(ServerRouteTestSuite, getCGIRoute_KO) {
    EXPECT_THROW(a.getCGIRoute("/test/index.html").getLocation(),  ServerGetRoute<Route>::RouteNotFoundWarn);
    EXPECT_THROW(a.getCGIRoute("/test/upload.txt").getLocation(),  ServerGetRoute<Route>::RouteNotFoundWarn);
    EXPECT_THROW(a.getCGIRoute("/test/delete.png").getLocation(),  ServerGetRoute<Route>::RouteNotFoundWarn);
    EXPECT_THROW(a.getCGIRoute("/emptyy").getLocation(),  ServerGetRoute<Route>::RouteNotFoundWarn);
    EXPECT_THROW(a.getCGIRoute("/space_travel.d").getLocation(),  ServerGetRoute<Route>::RouteNotFoundWarn);
    EXPECT_THROW(a.getCGIRoute("/*/diff.txt").getLocation(),  ServerGetRoute<Route>::RouteNotFoundWarn);
    EXPECT_THROW(a.getCGIRoute("/test/diff/delete.png").getLocation(), ServerGetRoute<Route>::RouteNotFoundWarn);
}

TEST_F(ServerRouteTestSuite, getCGIRoute_OK) {
    EXPECT_EQ(a.getCGIRoute("/test/delete.bla").getLocation(), "/test");
    EXPECT_EQ(a.getCGIRoute("/test/diff/delete.bla").getLocation(), "/test");
    EXPECT_EQ(a.getCGIRoute("/test/diff/deepdiff/delete.bla").getLocation(), "/test");
    EXPECT_EQ(a.getCGIRoute("/space_travel.bla/some/thing").getLocation(), "/test");
    EXPECT_EQ(a.getCGIRoute("/test/delete.diff").getLocation(), "/test/diff/deepdiff");
    EXPECT_EQ(a.getCGIRoute("/test/diff/delete.diff").getLocation(), "/test/diff/deepdiff");
    EXPECT_EQ(a.getCGIRoute("/test/diff/deepdiff/delete.diff").getLocation(), "/test/diff/deepdiff");
    EXPECT_EQ(a.getCGIRoute("/space_travel.diff/search/ici?ici=coucou").getLocation(), "/test/diff/deepdiff");
}
