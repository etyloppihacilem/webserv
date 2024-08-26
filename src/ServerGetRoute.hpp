/* #####################################################################################################################

               """          ServerGetRoute.hpp
        -\-    _|__
         |\___/  . \        Created on 21 Aug. 2024 at 15:23
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_SRC_SERVERGETROUTE_HPP_
#define INCLUDE_SRC_SERVERGETROUTE_HPP_

#include "Route.hpp"
#include <exception>
#include <map>
#include <string>

template < class RouteClass = Route >
class ServerGetRoute {
    public:
        ServerGetRoute();
        ~ServerGetRoute();

        const RouteClass &getRoute(const std::string &path) const;
        // const RouteClass &getRoute2(const std::string &path) const;
        std::string       getUploadLocation(const std::string &path) const;
        bool              hasRoute(const std::string &path) const;

        class RouteNotFoundWarn : public std::exception {
            public:
                RouteNotFoundWarn(std::string message = "") throw() : _message(message) {}

                virtual ~RouteNotFoundWarn() throw() {}

                const char *what() const throw() { return _message.c_str(); }

            private:
                std::string _message;
        };

    protected:
        std::map< std::string, RouteClass > _routes;
};

#endif // INCLUDE_SRC_SERVERGETROUTE_HPP_
