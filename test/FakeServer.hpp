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
#include "Logger.hpp"
#include <cstddef>
#include <exception>
#include <map>
#include <ostream>
#include <string>

class ResponseBuildStateFixture;
class LocationTestFixture;

class FakeServer {
    public:
        FakeServer() {}
        FakeServer(int) {};

        FakeRoute &getRoute(const std::string &path) {
            if (path[0] != '/') {
                warn.log() << "target '" << path
                           << "' is not starting with '/', wrong target. Choosing default route '/'." << std::endl;
                if (hasRoute("/"))
                    return _routes["/"];
                error.log() << "No default route with wrong target (not starting with /): '" << path << "'"
                            << std::endl;
                throw std::exception();
            }

            size_t      i          = path.find('/');
            std::string last_found = "/";
            std::string testing    = path.substr(0, i + 1);
            bool        eos        = false; // end of string
            // THIS IS NOT A REAL FUNCTION, FIX THE REAL ONE
            while (hasRoute(testing)) {
                last_found = testing;
                if (!eos)
                    i = path.find('/', i + 1);
                else
                    break;
                if (i == path.npos) {
                    eos = true;
                    i   = path.length();
                }
                testing = path.substr(0, i);
            }
            return _routes[last_found]; // will return "/" route if default
        }

        inline bool hasRoute(const std::string &path) const {
            return _routes.find(path) == _routes.end() ? false : true;
        }

        const std::map<HttpCode, std::string> &getErrorPages() const { return _error_pages; }

    private:
        std::map<std::string, FakeRoute> _routes;
        std::map<HttpCode, std::string>  _error_pages;

        friend ResponseBuildStateFixture;
        friend LocationTestFixture;
};

#endif // INCLUDE_TEST_MOCKSERVER_HPP_
