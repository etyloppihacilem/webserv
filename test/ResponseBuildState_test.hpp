/* #####################################################################################################################

               """          ResponseBuildState_test.hpp
        -\-    _|__
         |\___/  . \        Created on 28 Jul. 2024 at 18:18
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_TEST_RESPONSEBUILDSTATE_TEST_HPP_
#define INCLUDE_TEST_RESPONSEBUILDSTATE_TEST_HPP_

#include "ClientRequest.hpp"
#include "FakeRoute.hpp"
#include "FakeServer.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "ProcessState.hpp"
#include "ResponseBuildState.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "global_test.h"
#include "gtest/gtest.h"
#include <cstdio>
#include <exception>
#include <fstream>
#include <ios>
#include <map>
#include <ostream>
#include <string>
#include <tuple>

typedef enum e_strategies {
    aucune = 0,
    tCGIStrategy,
    tDeleteStrategy,
    tErrorStrategy,
    tGetFileStrategy,
    tGetIndexStrategy,
    tRedirectStrategy,
    tUploadStrategy,
} strategies;

typedef enum e_rbs {
    tname = 0, ///< Name for tests
    tdata,     ///< Data to build request (it should be tested in TotalRequest)
    ttype,     ///< Type of ResponseBuildingStrategy
    tstatus,   ///< Status of ResponseBuildingStrategy
    theaders,  ///< Headers
    thavebody, ///< Have body
    tbody,     ///< Body content
    tclean,
} t_rbs;

typedef std::tuple<
    std::string,                        ///< Name for tests
    std::string,                        ///< Data to build request (it should be tested in TotalRequest)
    strategies,                         ///< Type of ResponseBuildingStrategy
    HttpCode,                           ///< Status of ResponseBuildingStrategy
    std::map<std::string, std::string>, ///< Headers
    bool,                               ///< Have body
    std::string,                        ///< Body content
    bool                                ///< clean testing content
    >
    d_rbs;

// TODO: in progress but check that everything that does not need a read is done in one call of every
// epoll protected function.

class ResponseBuildStateFixture : public ::testing::TestWithParam<d_rbs> {
    public:
        ResponseBuildStateFixture() : _state(0), _strategy(0), _request(0) {}

        void SetUp() override {
            if (std::get<tclean>(GetParam()))
                clean();
            std::string buf = std::get<tdata>(GetParam());
            try {
                _request = new ClientRequest(0);
                _request->parse_header(buf); // as everything is in buff, no call to read_body() is ever needed
            } catch (std::exception &e) {
                FAIL() << "Could not build request." << e.what();
            }
            if (_request->get_status() != OK)
                FAIL() << "_request has error !";
            try {
                _state = new ResponseBuildState<FakeServer, FakeRoute>(0, _request, _server);
            } catch (std::exception &e) {
                FAIL() << "Could not build state." << e.what();
            }
            ASSERT_EQ(_state->process(), ready);
            _strategy = _state->get_response_strategy();
            // _strategy->build_response(); // at least once without read worry
            ASSERT_NE(_strategy, (void *) 0);
        }

        static void clean() {
            std::remove("www/test/upload.txt");
            std::remove("www/test/upload_a.md");
            std::remove("www/test/delete.png");
            {
                std::fstream f;
                f.open("www/test/delete.png", std::ios_base::out | std::ios_base::trunc);
                if (f.is_open())
                    f.close();
                else
                    GTEST_FAIL() << "Could not create file delete.png" << std::endl;
            }
            {
                std::fstream f;
                f.open("www/test/upload_a.md", std::ios_base::out | std::ios_base::trunc);
                if (f.is_open()) {
                    f << "# Super fichier texte\n\n";
                    f.close();
                } else
                    GTEST_FAIL() << "Could not create file upload_a.md" << std::endl;
            }
        }

        void TearDown() override {
            if (_state)
                delete _state;
            if (_request)
                delete _request;
            if (_strategy)
                delete _strategy;
            if (std::get<tclean>(GetParam()))
                clean();
        }

        static void TearDownTestSuite() { clean(); }

        static void SetUpTestSuite() {
            if (!filesystem_tests)
                GTEST_SKIP();
            _server._routes["/"] = FakeRoute(
                OK, true, false, false, false, true, "", "", "/", "", "www", "www/uploads", { GET, POST },
                { "index.html" }
            );
            _server._routes["/images"] = FakeRoute(
                OK, false, false, false, false, false, "", "", "/images", "", "www/images", "", { GET },
                { "index.html" }
            );
            _server._routes["/images/png/delete"] = FakeRoute(
                OK, false, false, false, false, false, "", "", "/images/png/delete", "", "www/images/png/delete", "",
                { GET, DELETE }, { "index.html" }
            );
            _server._routes["/forms"] = FakeRoute(
                OK, true, false, false, false, false, "", "", "/forms", "", "www/forms", "", { GET }, { "index.html" }
            );
            _server._routes["/space_travel"] = FakeRoute(
                MovedPermanently, false, false, false, true, false, "", "", "/space_travel",
                "/star_wars/milky_way.html", "www", "", { GET }, { "index.html" }
            );
            _server._routes["/forms/upload_form"] = FakeRoute(
                OK, false, false, false, false, true, "", "", "/forms/upload_form", "", "www/upload_form",
                "www/uploads", { GET, POST }, { "index.html" }
            );
            _server._routes["/forms/kill_form"] = FakeRoute(
                OK, true, false, false, false, false, "", "", "/forms/kill_form", "", "www/forms/kill_form", "",
                { GET }, { "index.html" }
            );
            _server._routes["/forms/love_form"] = FakeRoute(
                OK, true, false, false, false, false, "", "", "/forms/love_form", "", "www/forms/love_form", "",
                { GET }, { "index.html" }
            );
            _server._routes["/python"] = FakeRoute(
                OK, false, true, true, false, false, ".py", "/usr/bin/python3", "/python", "", "cgi-bin/python", "",
                { GET, POST }, { "diff.html" }
            );
            _server._routes["/perl"] = FakeRoute(
                OK, false, true, true, false, false, ".pl", "/usr/bin/perl", "/perl", "", "cgi-bin/perl", "", { GET },
                { "index.html" }
            );
            _server._routes["/php"] = FakeRoute(
                OK, false, true, true, false, false, ".php", "/usr/bin/php", "/php", "", "cgi-bin/perl", "", { GET },
                { "index.html" }
            );
            _server._routes["/test"] = FakeRoute(
                OK, true, false, false, false, false, "", "", "/test", "", "www/test", "", { GET, POST, DELETE },
                { "lindex.html" }
            );
            _server._routes["/space_corridor"] = FakeRoute(
                TemporaryRedirect, false, false, false, true, false, "", "", "/space_corridor",
                "http://space_corridor.fr/", "www", "", { GET }, { "index.html" }
            );
        }

    protected:
        static FakeServer                          _server;
        ResponseBuildState<FakeServer, FakeRoute> *_state;
        ResponseBuildingStrategy                  *_strategy;
        ClientRequest                             *_request;
};

#endif // INCLUDE_TEST_RESPONSEBUILDSTATE_TEST_HPP_
