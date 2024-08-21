/* #####################################################################################################################

               """          ServerGetRoute.cpp
        -\-    _|__
         |\___/  . \        Created on 21 Aug. 2024 at 15:23
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ServerGetRoute.hpp"
#include "Logger.hpp"
#include "Route.hpp"
#include <cstddef>
#include <ostream>
#include <string>

template < class RouteClass >
ServerGetRoute< RouteClass >::ServerGetRoute() {}

template < class RouteClass >
ServerGetRoute< RouteClass >::~ServerGetRoute() {}

template < class RouteClass >
bool ServerGetRoute< RouteClass >::hasRoute(const std::string &path) const {
    return _routes.find(path) == _routes.end() ? false : true;
}

// template < class RouteClass >
// const RouteClass &ServerGetRoute< RouteClass >::getRoute(const std::string &path) const {
//     if (path[0] != '/') {
//         warn.log() << "target '" << path << "' is not starting with '/', wrong target. Choosing default route '/'."
//                    << std::endl;
//         if (hasRoute("/"))
//             return _routes.at("/");
//         error.log() << "No default route with wrong target (not starting with /): '" << path << "'" << std::endl;
//         throw RouteNotFoundWarn(path);
//     }
//
//     size_t      nextSlash = path.find('/');
//     std::string lastFound = "/";
//     std::string testing   = path.substr(0, nextSlash + 1);
//
//     while (hasRoute(testing)) {
//         lastFound = testing;
//         if (testing.size() == path.size())
//             break;
//         nextSlash = path.find('/', nextSlash + 1);
//         if (nextSlash == std::string::npos) {
//             testing = path;
//             continue;
//         }
//         testing = path.substr(0, nextSlash);
//     }
//
//     return _routes.at(lastFound); // will return "/" route if default
// }

template < class RouteClass >
const RouteClass &ServerGetRoute< RouteClass >::getRoute(const std::string &path) const {
    if (path[0] != '/') {
        warn.log() << "target '" << path << "' is not starting with '/', wrong target. Choosing default route '/'."
                   << std::endl;
        if (hasRoute("/"))
            return _routes.at("/");
        error.log() << "No default route with wrong target (not starting with /): '" << path << "'" << std::endl;
        throw RouteNotFoundWarn(path);
    }

    size_t      nextSlash = 0;
    std::string lastFound = "/";
    std::string testing = "/";

    while (hasRoute(testing)) {
        lastFound = testing;
        if (testing.size() == path.size())
            break;
        nextSlash = path.find('/', nextSlash + 1);
        testing   = path;
        if (nextSlash != std::string::npos)
            testing.resize(nextSlash);
    }

    return _routes.at(lastFound); // will return "/" route if default
}

template class ServerGetRoute<>;

#ifdef TESTING
# include "FakeRoute.hpp"
template class ServerGetRoute< FakeRoute >;
#endif
