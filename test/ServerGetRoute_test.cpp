/* #####################################################################################################################

               """          ServerGetRoute_test.cpp
        -\-    _|__
         |\___/  . \        Created on 21 Aug. 2024 at 17:24
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

 #include "gtest/gtest.h"
 #include "Logger.hpp"
#include "Server.hpp"
#include "ServerConfFields.hpp"
#include "StringTokenizer.hpp"

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
            location = Field("/test", StringTokenizer("upload_path|/upload;", '|'));
            a.addRoute(location);
            location = Field("/test/diff", StringTokenizer("upload_path|/upload_diff;", '|'));
            a.addRoute(location);
            location = Field("/space_travel", StringTokenizer("", '|'));
            a.addRoute(location);
        }

        ~ServerRouteTestSuite() { info.enable(); }

        Server a;
        Field location;
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

//TODO:
/**/
/*TEST_F(ServerRouteTestSuite, getRoute_KO) {*/
/*    target        */
/*}*/

TEST_F(ServerRouteTestSuite, getRoute_KO) {
    EXPECT_EQ(a.getRoute("/test/upload.txt").getLocation(), "/test");
    EXPECT_EQ(a.getRoute("/test/delete.png").getLocation(), "/test");
    EXPECT_EQ(a.getRoute("/test/diff/delete.png").getLocation(), "/test/diff");
    EXPECT_EQ(a.getRoute("/space_travel/some/thing").getLocation(), "/space_travel");
    EXPECT_EQ(a.getRoute("/space_travel/search/ici?ici=coucou").getLocation(), "/space_travel");
}

// TODO: getCGIRoute
