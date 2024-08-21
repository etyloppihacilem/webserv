/* #####################################################################################################################

               """          MockServer.hpp
        -\-    _|__
         |\___/  . \        Created on 28 Jul. 2024 at 16:51
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_TEST_MOCKSERVER_HPP_
#define INCLUDE_TEST_MOCKSERVER_HPP_

#include "FakeRoute.hpp"
#include "HttpStatusCodes.hpp"
#include "Route.hpp"
#include "ServerGetRoute.hpp"
#include <map>
#include <string>

class ResponseBuildStateFixture;
class LocationTestFixture;

class FakeServer : public ServerGetRoute<FakeRoute> {
    public:
        FakeServer() {}

        FakeServer(int) {};

        const std::map< HttpCode, std::string > &getErrorPages() const { return _error_pages; }

    private:
        std::map< HttpCode, std::string > _error_pages;

        friend ResponseBuildStateFixture;
        friend LocationTestFixture;
};

#endif // INCLUDE_TEST_MOCKSERVER_HPP_
