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
#include "gtest/gtest.h"
#include <exception>
#include <map>
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
} t_rbs;

typedef std::tuple<
    std::string,                        ///< Name for tests
    std::string,                        ///< Data to build request (it should be tested in TotalRequest)
    strategies,                         ///< Type of ResponseBuildingStrategy
    HttpCode,                           ///< Status of ResponseBuildingStrategy
    std::map<std::string, std::string>, ///< Headers
    bool,                               ///< Have body
    std::string                         ///< Body content
    >
    d_rbs;

// TODO: in progress but check that everything that does not need a read is done in one call of every
// epoll protected function.

class ResponseBuildStateFixture : public ::testing::TestWithParam<d_rbs> {
    public:
        ResponseBuildStateFixture() {}

        void SetUp() override {
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
            ASSERT_NE(_strategy, (void *) 0);
        }

        void TearDown() override {
            if (_state)
                delete _state;
            if (_request)
                delete _request;
            if (_strategy)
                delete _strategy;
        }

        static void SetUpTestSuite() {
            _server._routes["/"] = FakeRoute(
                OK, true, false, false, false, true, "", "", "/", "", "www", "www/uploads", { GET, POST },
                { "index.html" }
            );
            _server._routes["/images"] = FakeRoute(
                OK, false, false, false, false, false, "", "", "/images", "", "www", "", { GET }, { "index.html" }
            );
            _server._routes["/images/png/delete"] = FakeRoute(
                OK, false, false, false, false, false, "", "", "/images/png/delete", "", "www", "", { GET, DELETE },
                { "index.html" }
            );
            _server._routes["/forms"] = FakeRoute(
                OK, true, false, false, false, false, "", "", "/forms", "", "www", "", { GET }, { "index.html" }
            );
            _server._routes["/space_travel"] = FakeRoute(
                MovedPermanently, false, false, false, true, false, "", "", "/space_travel",
                "/star_wars/milky_way.html", "/var/www", "", { GET }, { "index.html" }
            );
            _server._routes["/forms/upload_form"] = FakeRoute(
                OK, false, false, false, false, true, "", "", "/forms/upload_form", "", "www", "www/uploads",
                { GET, POST }, { "index.html" }
            );
            _server._routes["/forms/kill_form"] = FakeRoute(
                OK, true, false, false, false, false, "", "", "/forms/kill_form", "", "www", "", { GET },
                { "index.html" }
            );
            _server._routes["/forms/love_form"] = FakeRoute(
                OK, true, false, false, false, false, "", "", "/forms/love_form", "", "www", "", { GET },
                { "index.html" }
            );
            _server._routes["/python"] = FakeRoute(
                OK, false, true, true, false, false, ".py", "/usr/bin/python3", "/python", "", "cgi-bin", "",
                { GET, POST }, { "index.html" }
            );
            _server._routes["/perl"] = FakeRoute(
                OK, false, true, true, false, false, ".pl", "/usr/bin/perl", "/perl", "", "cgi-bin", "", { GET },
                { "index.html" }
            );
            _server._routes["/php"] = FakeRoute(
                OK, false, true, true, false, false, ".php", "/usr/bin/php", "/php", "", "cgi-bin", "", { GET },
                { "index.html" }
            );
        }

    protected:
        static FakeServer                          _server;
        ResponseBuildState<FakeServer, FakeRoute> *_state;
        ResponseBuildingStrategy                  *_strategy;
        ClientRequest                             *_request;
};

#endif // INCLUDE_TEST_RESPONSEBUILDSTATE_TEST_HPP_
