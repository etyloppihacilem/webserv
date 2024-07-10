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
#include "HttpStatusCodes.hpp"
#include "Route.hpp"
#include "Server.hpp"
#include <string>

Location::Location(ClientRequest &request, Server &server):
    _status_code(OK) {
    extern Route &route; // this is to get the reference out of try scope

    try {
        Route &route = server.getRoute(request.get_target());

        (void) route; // this is not used here BUT extern so it is use out of scope
    } catch (Server::RouteNotFoundWarn &e) {
        throw HttpError(InternalServerError); // this is to prevent use of route out of scope if not defined
    }
    (void) route;
}

Location::~Location() {}

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

bool Location::is_error() const {
    return _is_error;
}

HttpCode Location::get_status_code() const {
    return _status_code;
}

std::string Location::get_path() const {
    return _path;
}
