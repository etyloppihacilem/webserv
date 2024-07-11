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
#include <algorithm>
#include <cerrno>
#include <ostream>
#include <string>
#include <sys/stat.h>
#include <vector>

// TODO ask nicolas if it is possible to redefine error pages on different locations

Location::Location(ClientRequest &request, Server &server):
    _status_code(OK) {
    extern Route    &route; // this is to get the reference out of try scope
    std::string     &target = request.get_target();

    try {
        Route &route = server.getRoute(target);

        (void) route; // this is not used here BUT extern so it is use out of scope
    } catch (Server::RouteNotFoundWarn &e) {
        throw HttpError(InternalServerError); // this is to prevent use of route out of scope if not defined
    }
    build_path(request, route); // TODO CHECK IF REDIRECT ??????

    struct stat buf;

    if (stat(target.c_str(), &buf) != 0) {
        if (errno == ENOENT || errno == ENOTDIR)
            throw HttpError(NotFound); // there's nothing to see
        if (errno == EACCES)
            throw HttpError(Forbidden);
        if (errno == ENAMETOOLONG)
            throw HttpError(URITooLong);
        error.log() << "Stat failed to check target '" << target << "' using route '" << route.getLocation()
                    << "' at location (path) '" << _path << "'." << std::endl;
        throw HttpError(InternalServerError);
    }
    {
        const std::vector<HttpMethod> &methods = route.getMethods();

        _is_get     = std::find(methods.begin(), methods.end(), GET) != methods.end();
        _is_post    = std::find(methods.begin(), methods.end(), POST) != methods.end();
        _is_delete  = std::find(methods.begin(), methods.end(), DELETE) != methods.end();
    }
    if (S_ISDIR(buf.st_mode)) {
        const std::vector<std::string>  &indexs     = route.getIndexPage();
        const std::string               trailing    = (*_path.rbegin() == '/' ? "" : "/");

        for (std::vector<std::string>::const_iterator it = indexs.begin(); it != indexs.end(); it++) {
            std::string index_path = _path + trailing + *it;

            if (stat(_path.c_str(), &buf) == 0) {
                if (!S_ISREG(buf.st_mode)) { // if index.html or so is not a file.
                    warn.log() << *it << " is not a file, so cannot be a index." << std::endl;
                    continue;
                }
                _path       = index_path;
                _is_file    = true;
                return; // index file found and telling to read it
            }
            if (errno == ENOENT)
                continue; // file not found so searching forward
            if (errno == EACCES)
                throw HttpError(Forbidden); // file found but won't open
            error.log() << "Cannot stat index file at " << index_path << "and unknown error occured." << std::endl;
            throw HttpError(InternalServerError); // anything else is sus
        }
        _autoindex = route.hasAutoindex(); // if nothing else found
    } else {
        // TODO HERE check if CGI
        _is_file = true; // not a dir
    }
}

Location::~Location() {}

void Location::build_path(ClientRequest &request, Route &route) {
    _path = request.get_target();
    _path.replace(0, route.getLocation().length(), route.getRootDir());
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

bool Location::is_put() const {
    return _is_put;
}

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
