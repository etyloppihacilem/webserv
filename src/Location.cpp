/* #############################################################################

               """          Location.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jul. 2024 at 10:19
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Location.hpp"
#include "ClientRequest.hpp"
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

Location::Location(ClientRequest &request, Server &server):
    _status_code(OK) {
    Route       *route; // this is to get the reference out of try scope
    std::string target = request.get_target();
    struct stat buf;

    try {
        route = &server.getRoute(target);
    } catch (Server::RouteNotFoundWarn &e) {
        throw HttpError(InternalServerError); // this is to prevent use of route out of scope if not defined
    }
    if (route->hasRedir()) {
        _status_code    = route->getRedirCode();
        _is_redirect    = true;
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

    _is_get     = std::find(methods.begin(), methods.end(), GET) != methods.end();
    _is_post    = std::find(methods.begin(), methods.end(), POST) != methods.end();
    _is_delete  = std::find(methods.begin(), methods.end(), DELETE) != methods.end();
    // _is_put  = std::find(methods.begin(), methods.end(), PUT) != methods.end(); // not implemented yet
    if (S_ISDIR(buf.st_mode)) {         // in case target is a directory
        if (find_index(*route, buf))    // if index file is found
            return;
        _autoindex = route->hasAutoindex(); // if nothing else found
    } else {                                // in case target is anything but a directory
        _is_file = true;                    // not a dir
        if (route->hasCgiExtension() && extract_extension(_path) == route->getCgiExtension())
            setup_cgi(*route);
    }
} // 44 lines its a bit better

Location::~Location() {}

/**
  Function used to find index in a repository.
  */
bool Location::find_index(const Route &route, struct stat &buf) {
    const std::vector<std::string> &indexs = route.getIndexPage();
    const std::string trailing = (*_path.rbegin() == '/' ? "" : "/");

    for (std::vector<std::string>::const_iterator it = indexs.begin(); it != indexs.end(); it++) {
        std::string index_path = _path + trailing + *it;

        if (stat(_path.c_str(), &buf) == 0) {
            if (!S_ISREG(buf.st_mode)) {     // if index.html or so is not a file.
                warn.log() << *it << " is not a file, so cannot be an index. Continuing." << std::endl;
                continue;
            }
            _path       = index_path;
            _is_file    = true;
            if (route.hasCgiExtension() && extract_extension(_path) == route.getCgiExtension())
                setup_cgi(route);
            return true;     // index file found and telling to read it
        }
        if (errno == ENOENT)
            continue;     // file not found so searching forward
        if (errno == EACCES)
            throw HttpError(Forbidden);     // file found but won't open
        error.log() << "Cannot stat index file at " << index_path << "and unknown error occured." << std::endl;
        throw HttpError(InternalServerError);     // anything else is sus
    }
    return false;
}

/**
  Used to build path of resource
  */
void Location::build_path(const std::string &target, const Route &route) {
    _path       = target;
    _path_info  = target;
    _route_path = route.getRootDir();
    _path.replace(0, route.getLocation().length(), _route_path);
    _path_info.replace(0, route.getLocation().length(), "");
}

/**
  Used to build redirect target (stored in path)
  if redirect last chat is a '/', route's location is replaced by the redirection location.
  if not, redirection is considered to redirect on a file and the whole target is replaced by the new location.
  */
void Location::build_path(const std::string &target, const Route &route, const std::string &redirect) {
    if (*redirect.rbegin() != '/') {
        _path = redirect;
        return;
    } else {
        _path = target;
        _path.replace(0, route.getLocation().length(), redirect);
    }
}

/**
  Setup cgi location info for a given route.
  */
void Location::setup_cgi(const Route &route) {
    if (!route.hasCgiPath() || !route.hasCgiExtension())
        return;  // nothing to configure because cgi is incomplete
    _is_cgi     = true;
    _cgi_path   = route.getCgiPath();
}

bool Location::is_get() const {
    return _is_get;
}

bool Location::is_post() const {
    return _is_post;
}

bool Location::is_delete() const {
    return _is_delete;
}

// bool Location::is_put() const {
//     return _is_put;
// }

bool Location::has_autoindex() const {
    return _autoindex;
}

bool Location::is_file() const {
    return _is_file;
}

bool Location::is_cgi() const {
    return _is_cgi;
}

bool Location::is_redirect() const {
    return _is_redirect;
}

HttpCode Location::get_status_code() const {
    return _status_code;
}

std::string Location::get_path() const {
    return _path;
}
