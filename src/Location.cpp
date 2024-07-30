/* #############################################################################

               """          Location.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jul. 2024 at 10:19
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Location.hpp"
#include "ClientRequest.hpp"
#include "HttpError.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "Route.hpp"
#include "Server.hpp"
#include "StringUtils.hpp"
#include <algorithm>
#include <cerrno>
#include <ostream>
#include <string>
#include <sys/stat.h>
#include <vector>

template <class ServerClass, class RouteClass>
Location<ServerClass, RouteClass>::Location() {}

template <class ServerClass, class RouteClass>
Location<ServerClass, RouteClass>::Location(const std::string &target, ServerClass &server) : _status_code(OK) {
    RouteClass *route; // this is to get the reference out of try scope
    struct stat buf;

    try {
        route = &server.getRoute(target);
    } catch (Server::RouteNotFoundWarn &e) {
        throw HttpError(InternalServerError); // this is to prevent use of route out of scope if not defined
    }
    if (route->hasRedir()) {
        _status_code = route->getRedirCode();
        _is_redirect = true;
        build_path(target, *route, route->getRedirPage());
        return;
    }
    build_path(target, *route);
    if (stat(target.c_str(), &buf) != 0) {
        if (errno == ENOENT || errno == ENOTDIR)
            throw HttpError(NotFound); // there's nothing to be found
        if (errno == EACCES)
            throw HttpError(Forbidden);
        if (errno == ENAMETOOLONG)
            throw HttpError(URITooLong);
        error.log() << "Stat failed to check target '" << target << "' using route '" << route->getLocation()
                    << "' at location (path) '" << _path << "'." << std::endl;
        throw HttpError(InternalServerError);
    }

    const std::vector<HttpMethod> &methods = route->getMethods();

    _is_get    = std::find(methods.begin(), methods.end(), GET) != methods.end();
    _is_post   = std::find(methods.begin(), methods.end(), POST) != methods.end();
    _is_delete = std::find(methods.begin(), methods.end(), DELETE) != methods.end();
    // _is_put  = std::find(methods.begin(), methods.end(), PUT) != methods.end(); // not implemented yet
    if (S_ISDIR(buf.st_mode)) {      // in case target is a directory
        if (find_index(*route, buf)) // if index file is found
            return;
        _autoindex = route->hasAutoindex(); // if nothing else found
    } else {                                // in case target is anything but a directory
        _is_file = true;                    // not a dir
        if (route->hasCgiExtension() && extract_extension(_path) == route->getCgiExtension())
            setup_cgi(*route);
    }
} // 44 lines its a bit better

template <class ServerClass, class RouteClass>
Location<ServerClass, RouteClass>::~Location() {}

/**
  Function used to find index in a repository.
  */
template <class ServerClass, class RouteClass>
bool Location<ServerClass, RouteClass>::find_index(const RouteClass &route, struct stat &buf) {
    const std::vector<std::string> &indexs   = route.getIndexPage();
    const std::string               trailing = (*_path.rbegin() == '/' ? "" : "/");

    for (std::vector<std::string>::const_iterator it = indexs.begin(); it != indexs.end(); it++) {
        std::string index_path = _path + trailing + *it;

        if (stat(_path.c_str(), &buf) == 0) {
            if (!S_ISREG(buf.st_mode)) { // if index.html or so is not a file.
                warn.log() << *it << " is not a file, so cannot be an index. Continuing." << std::endl;
                continue;
            }
            _path    = index_path;
            _is_file = true;
            if (route.hasCgiExtension() && extract_extension(_path) == route.getCgiExtension())
                setup_cgi(route);
            return true; // index file found and telling to read it
        }
        if (errno == ENOENT)
            continue; // file not found so searching forward
        if (errno == EACCES)
            throw HttpError(Forbidden); // file found but won't open
        error.log() << "Cannot stat index file at " << index_path << "and unknown error occured." << std::endl;
        throw HttpError(InternalServerError); // anything else is sus
    }
    return false;
}

/**
  Used to build path of resource
  */
template <class ServerClass, class RouteClass>
void Location<ServerClass, RouteClass>::build_path(const std::string &target, const RouteClass &route) {
    _path       = target;
    _path_info  = target;
    _route_path = route.getRootDir();
    _path.replace(0, route.getLocation().length(), _route_path);
    _path_info.replace(0, route.getLocation().length(), "");
}

/**
  Used to build redirect target (stored in path)
  if redirect last char is a '/', route's location is replaced by the redirection location.
  if not, redirection is considered to redirect on a file and the whole target is replaced by the new location.
  */
template <class ServerClass, class RouteClass>
void Location<ServerClass, RouteClass>::build_path(
    const std::string &target,
    const RouteClass  &route,
    const std::string &redirect
) {
    if (*redirect.rbegin() != '/') {
        _path = redirect;
        return;
    } else {
        _path = target;
        _path.replace(0, route.getLocation().length() + 1, redirect);
    }
}

/**
  Setup cgi location info for a given route.
  */
template <class ServerClass, class RouteClass>
void Location<ServerClass, RouteClass>::setup_cgi(const RouteClass &route) {
    if (!route.hasCgiPath() || !route.hasCgiExtension())
        return; // nothing to configure because cgi is incomplete
    _is_cgi   = true;
    _cgi_path = route.getCgiPath();
}

template <class ServerClass, class RouteClass>
bool Location<ServerClass, RouteClass>::is_get() const {
    return _is_get;
}

template <class ServerClass, class RouteClass>
bool Location<ServerClass, RouteClass>::is_post() const {
    return _is_post;
}

template <class ServerClass, class RouteClass>
bool Location<ServerClass, RouteClass>::is_delete() const {
    return _is_delete;
}

// template <class ServerClass, class RouteClass>
// bool Location<ServerClass, RouteClass>::is_put() const {
//      return _is_put;
//  }

template <class ServerClass, class RouteClass>
bool Location<ServerClass, RouteClass>::has_autoindex() const {
    return _autoindex;
}

template <class ServerClass, class RouteClass>
bool Location<ServerClass, RouteClass>::is_file() const {
    return _is_file;
}

template <class ServerClass, class RouteClass>
bool Location<ServerClass, RouteClass>::is_cgi() const {
    return _is_cgi;
}

template <class ServerClass, class RouteClass>
bool Location<ServerClass, RouteClass>::is_redirect() const {
    return _is_redirect;
}

template <class ServerClass, class RouteClass>
HttpCode Location<ServerClass, RouteClass>::get_status_code() const {
    return _status_code;
}

template <class ServerClass, class RouteClass>
std::string Location<ServerClass, RouteClass>::get_path() const {
    return _path;
}

template <class ServerClass, class RouteClass>
std::string Location<ServerClass, RouteClass>::get_route_path() const {
    return _route_path;
}

template <class ServerClass, class RouteClass>
std::string Location<ServerClass, RouteClass>::get_path_info() const {
    return _path_info;
}

#ifdef TESTING
# include "FakeRoute.hpp"
# include "FakeServer.hpp"
template class Location<FakeServer, FakeRoute>;
#endif

