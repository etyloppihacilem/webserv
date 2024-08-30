/* #####################################################################################################################

               """          ServerGetRoute.cpp
        -\-    _|__
         |\___/  . \        Created on 21 Aug. 2024 at 15:23
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ServerGetRoute.hpp"
#include "Logger.hpp"
#include "Path.hpp"
#include "Route.hpp"
#include "StringUtils.hpp"
#include <cstddef>
#include <map>
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
const RouteClass &ServerGetRoute< RouteClass >::getCGIRoute(const std::string &path) const {
    debug.log() << "Checking if " << path << " does contain any cgi specific extensions." << std::endl;
    size_t      nextSlash = (path.length() > 1 ? path.find("/", 1) : 0);
    std::string testing   = path.substr(0, nextSlash);
    while (nextSlash != path.npos) {
        std::string ext = extract_extension(testing);
        if (ext == "") // no extension
            continue;
       // condition extract_extension(testing)
        // when found call init and return
    }
    throw ;// if not found
}

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
    std::string testing   = "/";

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

template < class RouteClass >
std::string ServerGetRoute< RouteClass >::getUploadLocation(const std::string &path) const {
    typename std::map< std::string, RouteClass >::const_iterator
        it; // ig typename is required as RouteClass is a template
    typename std::map< std::string, RouteClass >::const_iterator
         best; // ig typename is required as RouteClass is a template
    bool found = false;
    Path ppath = path;
    Path pbest;
    for (it = _routes.begin(); it != _routes.end(); it++) {
        Path pit = it->second.getRootDir();
        if (ppath.in(pit)                                      // if path matches
            && (!found                                         // if not found yet update bc best is not initialized yet
                || pbest.str().length() < pit.str().length()   // and most precise on device
                || (pbest.str().length() == pit.str().length() // and if equal on device
                    && best->second.getLocation().length() < it->second.getLocation().length()))) // most precise URI
        {
            best  = it;
            pbest = pit; // Path lib is needed because of absolute paths
            found = true;
        }
    }
    std::string ret = ppath.str();
    ret.replace(0, pbest.str().length(), best->second.getLocation());
    return found ? ret : "";
}

template class ServerGetRoute<>;

#ifdef TESTING
# include "FakeRoute.hpp"
template class ServerGetRoute< FakeRoute >;
#endif
