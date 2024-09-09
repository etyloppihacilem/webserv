/* #####################################################################################################################

               """          Location_test.cpp
        -\-    _|__
         |\___/  . \        Created on 30 Jul. 2024 at 11:20
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "FakeRoute.hpp"
#include "FakeServer.hpp"
#include "Location.hpp"
#include "gtest/gtest.h"
#include <string>

#ifndef WORKDIR
# define WORKDIR "./www"
#endif

std::string FakeRoute::workdir = ""; // default, do not change here

TEST(LocationTestSuite, BuildPathTest) {
    Location< FakeServer, FakeRoute > loc;
    FakeRoute                         route;
    std::string                       original_target = "/index.html";
    std::string                       target          = "/target" + original_target;

    route._getRootDir  = "routeDirTEST";
    route._getLocation = "/target";
    loc.build_path(target, route);
    EXPECT_EQ(loc.get_route_path(), "routeDirTEST");
    EXPECT_EQ(loc.get_path(), "routeDirTEST" + original_target);
    EXPECT_EQ(loc.get_path_info(), "");
}

TEST(LocationTestSuite, BuildPathRedirectTest) {
    Location< FakeServer, FakeRoute > loc;
    std::string                       redirect = "coucou";
    std::string                       target   = "/index.html";
    FakeRoute                         route;
    route._getLocation = "/target";
    // with absolute redirection
    loc.build_path(target, route, redirect);
    EXPECT_EQ(loc.get_path(), redirect);
    // with relative redirection
    std::string ntarget = "/target" + target;
    redirect            = "/new_location";
    loc.build_path(ntarget, route, redirect + "/");
    EXPECT_EQ(loc.get_path(), redirect + target);
}
