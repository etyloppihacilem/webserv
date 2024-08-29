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
#include <algorithm>
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
    _is_get(false),
    _is_post(false),
    _is_delete(false),
    _autoindex(false),
    _is_file(false),
    _is_cgi(false),
    _is_redirect(false),
    _is_diff(false),
    _status_code(OK),
    _default_error(Forbidden) {}

template < class ServerClass, class RouteClass >
Location< ServerClass, RouteClass >::Location(const std::string &target, const ServerClass &server) :
    _is_get(false),
    _is_post(false),
    _is_delete(false),
    _autoindex(false),
    _is_file(false),
    _is_cgi(false),
    _is_redirect(false),
    _is_diff(false),
    _status_code(OK),
    _default_error(Forbidden) {
    struct stat buf;

    try {
        const RouteClass &route = server.getRoute(target);
        if (route.hasRedirSet()) {
            debug.log() << "Using route " << route.getLocation() << " to redirect" << std::endl;
            _status_code = route.getRedirCode();
            _is_redirect = true;
            build_path(target, route, route.getRedirPage());
            debug.log() << "Redirection location: " << _path << std::endl;
            return;
        }
        build_path(target, route);

        const std::set< HttpMethod > &methods = route.getMethods();

        _is_diff = route.getRootDir() != route.getUploadPath();
        _route   = route.getLocation();
        debug.log() << "Using route " << _route << std::endl;
        _is_get = std::find(methods.begin(), methods.end(), GET) != methods.end();
        debug.log() << "GET is enabled" << std::endl;
        _is_post = std::find(methods.begin(), methods.end(), POST) != methods.end();
        debug.log() << "POST is enabled" << std::endl;
        _is_delete = std::find(methods.begin(), methods.end(), DELETE) != methods.end();
        debug.log() << "DELETE is enabled" << std::endl;
        // _is_put  = std::find(methods.begin(), methods.end(), PUT) != methods.end(); // not implemented yet
        // debug.log() << "PUT is enabled." << std::endl;
        if (stat(_path.c_str(), &buf) != 0)
            switch (errno) {
                case ENOENT:
                case ENOTDIR:
                    _is_file = true; // it's a file if it doesnt exist
                    return;
                case EACCES:
                    info.log() << "Location: resource '" << target << "' " << strerror(errno) << ", sending "
                               << Forbidden << std::endl;
                    throw HttpError(Forbidden);
                case ENAMETOOLONG:
                    info.log() << "Location: resource '" << target << "' " << strerror(errno) << ", sending "
                               << Forbidden << std::endl;
                    throw HttpError(URITooLong);
                default:
                    error.log() << "Location: resource '" << target << "' at location '" << _path << "' using route '"
                                << route.getLocation() << "'" << strerror(errno) << ", sending " << InternalServerError
                                << std::endl;
                    throw HttpError(InternalServerError);
            } // default throws so nothing gets out of there
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
bool Location< ServerClass, RouteClass >::is_get() const {
    return _is_get;
}

template < class ServerClass, class RouteClass >
bool Location< ServerClass, RouteClass >::is_post() const {
    return _is_post;
}

template < class ServerClass, class RouteClass >
bool Location< ServerClass, RouteClass >::is_delete() const {
    return _is_delete;
}

// template <class ServerClass, class RouteClass>
// bool Location<ServerClass, RouteClass>::is_put() const {
//      return _is_put;
//  }

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

template class Location<>; // force compilation for this template (defaults)

#ifdef TESTING
# include "FakeRoute.hpp"
# include "FakeServer.hpp"
template class Location< FakeServer, FakeRoute >; // force compilation for test templates
#endif
