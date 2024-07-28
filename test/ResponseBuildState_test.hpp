/* #####################################################################################################################

               """          ResponseBuildState_test.hpp
        -\-    _|__
         |\___/  . \        Created on 28 Jul. 2024 at 18:18
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_TEST_RESPONSEBUILDSTATE_TEST_HPP_
#define INCLUDE_TEST_RESPONSEBUILDSTATE_TEST_HPP_

#include "FakeRoute.hpp"
#include "FakeServer.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <tuple>

typedef enum e_rbs {
    tname = 0,  ///< Name for tests
    tdata,      ///< Data to build request (it should be tested in TotalRequest)
    ttype,      ///< Type of ResponseBuildingStrategy
    tstatus,    ///< Status of ResponseBuildingStrategy
    theaders,   ///< Headers
    thavebody,  ///< Have body
    tbody,      ///< Body content
} t_rbs;

typedef enum e_strategies {
    aucune = 0,
    CGIStrategy,
    DeleteStrategy,
    ErrorStrategy,
    GetFileStrategy,
    GetIndexStrategy,
    RedirectStrategy,
    UploadStrategy
} strategies;

typedef std::tuple<std::string,             ///< Name for tests
        std::string,                        ///< Data to build request (it should be tested in TotalRequest)
        strategies,         ///< Type of ResponseBuildingStrategy
        HttpCode,                           ///< Status of ResponseBuildingStrategy
        std::map<std::string, std::string>, ///< Headers
        bool,                               ///< Have body
        std::string                         ///< Body content
        > d_rbs;

class ResponseBuildStateFixture: public ::testing::TestWithParam<d_rbs> {
    public:
        ResponseBuildStateFixture() {}

        void SetUp() override {

        }

        void TearDown() override {

        }

        static void SetUpTestSuite() {
            _server._routes["/"] = FakeRoute(OK,
                    true,
                    false,
                    false,
                    false,
                    true,
                    "",
                    "",
                    "/",
                    "",
                    "/var/www",
                    "/uploads",
        {
            GET, POST
        },
        {
            "index.html"
        });
            _server._routes["/images"] = FakeRoute(OK,
                    false,
                    false,
                    false,
                    false,
                    false,
                    "",
                    "",
                    "/images",
                    "",
                    "/var/www",
                    "",
        {
            GET
        },
        {
            "index.html"
        });
            _server._routes["/images/png/delete"] = FakeRoute(OK,
                    false,
                    false,
                    false,
                    false,
                    false,
                    "",
                    "",
                    "/images/png/delete",
                    "",
                    "/var/www",
                    "",
        {
            GET, DELETE
        },
        {
            "index.html"
        });
            _server._routes["/forms"] = FakeRoute(OK,
                    true,
                    false,
                    false,
                    false,
                    false,
                    "",
                    "",
                    "/forms",
                    "",
                    "/var/www",
                    "",
        {
            GET
        },
        {
            "index.html"
        });
            _server._routes["/space_travel"] = FakeRoute(MovedPermanently,
                    false,
                    false,
                    false,
                    true,
                    false,
                    "",
                    "",
                    "/space_travel",
                    "/star_wars/milky_way.html",
                    "/var/www",
                    "",
        {
            GET
        },
        {
            "index.html"
        });
            _server._routes["/forms/upload_form"] = FakeRoute(OK,
                    false,
                    false,
                    false,
                    false,
                    true,
                    "",
                    "",
                    "/forms/upload_form",
                    "",
                    "/var/www",
                    "/uploads",
        {
            GET, POST
        },
        {
            "index.html"
        });
            _server._routes["/forms/kill_form"] = FakeRoute(OK,
                    true,
                    false,
                    false,
                    false,
                    false,
                    "",
                    "",
                    "/forms/kill_form",
                    "",
                    "/var/www",
                    "",
        {
            GET
        },
        {
            "index.html"
        });
            _server._routes["/forms/love_form"] = FakeRoute(OK,
                    true,
                    false,
                    false,
                    false,
                    false,
                    "",
                    "",
                    "/forms/love_form",
                    "",
                    "/var/www",
                    "",
        {
            GET
        },
        {
            "index.html"
        });
            _server._routes["/python"] = FakeRoute(OK,
                    false,
                    true,
                    true,
                    false,
                    false,
                    ".py",
                    "/usr/bin/python3",
                    "/python",
                    "",
                    "/var/cgi-bin",
                    "",
        {
            GET, POST
        },
        {
            "index.html"
        });
            _server._routes["/perl"] = FakeRoute(OK,
                    false,
                    true,
                    true,
                    false,
                    false,
                    ".pl",
                    "/usr/bin/perl",
                    "/perl",
                    "",
                    "/var/cgi-bin",
                    "",
        {
            GET
        },
        {
            "index.html"
        });
            _server._routes["/php"] = FakeRoute(OK,
                    false,
                    true,
                    true,
                    false,
                    false,
                    ".php",
                    "/usr/bin/php",
                    "/php",
                    "",
                    "/var/cgi-bin",
                    "",
        {
            GET
        },
        {
            "index.html"
        });
        }

    protected:
        static FakeServer _server;
};


#endif  // INCLUDE_TEST_RESPONSEBUILDSTATE_TEST_HPP_
