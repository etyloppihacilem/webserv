/* #############################################################################

               """          Location.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jul. 2024 at 10:19
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Location.hpp"
#include "HttpError.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "Route.hpp"
#include "Server.hpp"
#include "StringUtils.hpp"
#include <cerrno>
#include <cstring>
#include <features.h>
#include <ostream>
#include <set>
#include <string>
#include <sys/stat.h>
#include <vector>

template < class ServerClass, class RouteClass >
Location< ServerClass, RouteClass >::Location() :
    _autoindex(false),
    _is_file(false),
    _is_cgi(false),
    _is_redirect(false),
    _is_diff(false),
    _status_code(OK),
    _default_error(Forbidden) {}

template < class ServerClass, class RouteClass >
Location< ServerClass, RouteClass >::Location(const std::string &target, const ServerClass &server) :
    _autoindex(false),
    _is_file(false),
    _is_cgi(false),
    _is_redirect(false),
    _is_diff(false),
    _status_code(OK),
    _default_error(Forbidden) {
    struct stat buf;
    if (check_cgi_glob(target, server))
        return;
    try {
        const RouteClass &route = server.getRoute(target);
        _route                  = route.getLocation();
        debug.log() << "Using route " << _route << std::endl;
        if (route.hasRedirSet()) {
            set_redir(target, route);
            return;
        }
        build_path(target, route);
        _is_diff = route.getRootDir() != route.getUploadPath();
        _methods = route.getMethods();
        stat_file(target, buf);
        debug.log() << "Target '" << target << "' exists." << std::endl;
        if (S_ISDIR(buf.st_mode)) { // in case target is a directory
            debug.log() << "Target is a directory" << std::endl;
            if (find_index(route, buf)) // if index file is found
                return;
            _autoindex = route.hasAutoindexSet(); // if nothing else found
            debug.log() << "Autoindex is " << (_autoindex ? "enabled" : "disabled") << std::endl;
        } else {             // in case target is anything but a directory
            _is_file = true; // not a dir
            debug.log() << "Target is a file" << std::endl;
            if (route.hasCgiExtensionSet() && extract_extension(_path) == route.getCgiExtension()) {
                debug.log() << "Route has cgi " << route.getCgiPath() << " and target correct extension "
                            << route.getCgiExtension() << std::endl;
                setup_cgi(route);
            }
        }
    } catch (Server::RouteNotFoundWarn &e) {
        fatal.log() << "Location: Route not found. Should NEVER happen, this means Server object is broken and "
                       "default Route '/' do not exist. Sending "
                    << InternalServerError << std::endl;
        throw HttpError(InternalServerError); // this is to prevent use of route out of scope if not defined
    }
} // 44 lines its a bit better

template < class ServerClass, class RouteClass >
Location< ServerClass, RouteClass >::~Location() {}

template < class ServerClass, class RouteClass >
void Location< ServerClass, RouteClass >::stat_file(const std::string &target, struct stat &buf) {
    if (stat(_path.c_str(), &buf) != 0)
        switch (errno) {
            case ENOENT:
            case ENOTDIR:
                _is_file = true; // it's a file if it doesnt exist
                return;
            case EACCES:
                info.log() << "Location: resource '" << target << "' " << strerror(errno) << ", sending " << Forbidden
                           << std::endl;
                throw HttpError(Forbidden);
            case ENAMETOOLONG:
                info.log() << "Location: resource '" << target << "' " << strerror(errno) << ", sending " << Forbidden
                           << std::endl;
                throw HttpError(URITooLong);
            default:
                error.log() << "Location: resource '" << target << "' at location '" << _path << "' " << strerror(errno)
                            << ", sending " << InternalServerError << std::endl;
                throw HttpError(InternalServerError);
        } // default throws so nothing gets out of there
}

template < class ServerClass, class RouteClass >
void Location< ServerClass, RouteClass >::set_redir(const std::string &target, const RouteClass &route) {
    debug.log() << "Using route " << route.getLocation() << " to redirect" << std::endl;
    _status_code = route.getRedirCode();
    _is_redirect = true;
    build_path(target, route, route.getRedirPage());
    debug.log() << "Redirection location: " << _path << std::endl;
}

template < class ServerClass, class RouteClass >
bool Location< ServerClass, RouteClass >::check_cgi_glob(const std::string &target, const ServerClass &server) {
    try {
        const RouteClass &route = server.getCGIRoute(target); // TODO: change type, is there for LSP
        debug.log() << "Using CGI glob route " << route.getLocation() << std::endl;
        init_cgi_glob(target, route);
        return true;
    } catch (Server::RouteNotFoundWarn &e) {
        return false;
    }
    return false;
}

template < class ServerClass, class RouteClass >
void Location< ServerClass, RouteClass >::init_cgi_glob(
    const std::string &target,
    const RouteClass  &route
) { // TODO: change type
    _is_cgi         = true;
    _methods        = route.getMethods();
    _route          = route.getLocation();
    _cgi_path       = route.getRootDir();
    std::string ext = route.getCgiExtension();
    size_t      sep;
    size_t      next_slash;
    do {
        sep        = target.find(ext, 0) + ext.length();
        next_slash = target.find("/", sep);
    } while (next_slash != 0 && next_slash != target.npos);
    _cgi_path   = route.getCgiPath();
    _path       = target.substr(0, sep);
    _route_path = route.getRootDir();
    _path_info  = target.substr(sep, target.length() - sep);
}

/**
  Function used to find index in a repository.
  */
template < class ServerClass, class RouteClass >
bool Location< ServerClass, RouteClass >::find_index(const RouteClass &route, struct stat &buf) {
    const std::vector< std::string > &indexs   = route.getIndexPage();
    const std::string                 trailing = (*_path.rbegin() == '/' ? "" : "/");

    debug.log() << "Searching for index file in " << _path << std::endl;
    for (std::vector< std::string >::const_iterator it = indexs.begin(); it != indexs.end(); it++) {
        std::string index_path = _path + trailing + *it;

        if (stat(index_path.c_str(), &buf) == 0) {
            debug.log() << "Trying " << index_path << std::endl;
            if (!S_ISREG(buf.st_mode)) { // if index.html or so is not a file.
                warn.log() << *it << " is not a file, so cannot be an index. Continuing." << std::endl;
                continue;
            }
            _path    = index_path;
            _is_file = true;
            debug.log() << "Index file " << _path << " was found" << std::endl;
            if (route.hasCgiExtensionSet() && extract_extension(_path) == route.getCgiExtension())
                setup_cgi(route);
            return true; // index file found and telling to read it
        }
        if (errno == ENOENT)
            continue; // file not found so searching forward
        if (errno == EACCES) {
            info.log() << "Index file " << index_path << " found,  " << strerror(errno) << ", sending " << Forbidden
                       << std::endl;
            throw HttpError(Forbidden); // file found but won't open
        }
        error.log() << "Cannot stat index file at " << index_path << "and unknown error occured: " << strerror(errno)
                    << std::endl;
        throw HttpError(InternalServerError); // anything else is sus
    }
    debug.log() << "No index file found in " << _path << std::endl;
    return false;
}

/**
  Used to build path of resource
  */
template < class ServerClass, class RouteClass >
void Location< ServerClass, RouteClass >::build_path(const std::string &target, const RouteClass &route) {
    _path        = target;
    _upload_path = target;
    _path_info   = target;
    _route_path  = route.getRootDir();
    if (*_route_path.rbegin() == '/')
        _route_path.resize(_route_path.length() - 1);
    if (_path[route.getLocation().length()] != '/' && _path != route.getLocation()) {
        _path.replace(0, route.getLocation().length() - 1, _route_path);
        _upload_path.replace(0, route.getLocation().length() - 1, route.getUploadPath());
    } else {
        _path.replace(0, route.getLocation().length(), _route_path);
        _upload_path.replace(0, route.getLocation().length(), route.getUploadPath());
    }
    _path_info.replace(0, route.getLocation().length(), "");
}

/**
  Used to build redirect target (stored in path)
  if redirect last char is a '/', route's location is replaced by the redirection location.
  if not, redirection is considered to redirect on a file and the whole target is replaced by the new location.
  */
template < class ServerClass, class RouteClass >
void Location< ServerClass, RouteClass >::build_path(
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
template < class ServerClass, class RouteClass >
void Location< ServerClass, RouteClass >::setup_cgi(const RouteClass &route) {
    if (!route.hasCgiPathSet() || !route.hasCgiExtensionSet())
        return; // nothing to configure because cgi is incomplete
    _is_cgi   = true;
    _cgi_path = route.getCgiPath();
}

template < class ServerClass, class RouteClass >
bool Location< ServerClass, RouteClass >::has_autoindex() const {
    return _autoindex;
}

template < class ServerClass, class RouteClass >
bool Location< ServerClass, RouteClass >::is_file() const {
    return _is_file;
}

template < class ServerClass, class RouteClass >
bool Location< ServerClass, RouteClass >::is_cgi() const {
    return _is_cgi;
}

template < class ServerClass, class RouteClass >
bool Location< ServerClass, RouteClass >::is_redirect() const {
    return _is_redirect;
}

template < class ServerClass, class RouteClass >
bool Location< ServerClass, RouteClass >::is_diff() const {
    return _is_diff;
}

template < class ServerClass, class RouteClass >
HttpCode Location< ServerClass, RouteClass >::get_status_code() const {
    return _status_code;
}

template < class ServerClass, class RouteClass >
const std::string &Location< ServerClass, RouteClass >::get_route() const {
    return _route;
}

template < class ServerClass, class RouteClass >
const std::string &Location< ServerClass, RouteClass >::get_path() const {
    return _path;
}

template < class ServerClass, class RouteClass >
const std::string &Location< ServerClass, RouteClass >::get_upload_path() const {
    return _upload_path;
}

template < class ServerClass, class RouteClass >
const std::string &Location< ServerClass, RouteClass >::get_route_path() const {
    return _route_path;
}

template < class ServerClass, class RouteClass >
const std::string &Location< ServerClass, RouteClass >::get_path_info() const {
    return _path_info;
}

template < class ServerClass, class RouteClass >
const std::string &Location< ServerClass, RouteClass >::get_cgi_path() const {
    return _cgi_path;
}

template < class ServerClass, class RouteClass >
bool Location< ServerClass, RouteClass >::has_method(HttpMethod method) const {
    return _methods.find(method) != _methods.end();
}

template class Location<>; // force compilation for this template (defaults)

#ifdef TESTING
# include "FakeRoute.hpp"
# include "FakeServer.hpp"
template class Location< FakeServer, FakeRoute >; // force compilation for test templates
#endif
