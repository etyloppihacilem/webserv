/* #############################################################################

               """          ResponseBuildState.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:12
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ResponseBuildState.hpp"
#include "BodyWriter.hpp"
#include "CGIStrategy.hpp"
#include "ClientRequest.hpp"
#include "DeleteStrategy.hpp"
#include "ErrorStrategy.hpp"
#include "GetFileStrategy.hpp"
#include "GetIndexStrategy.hpp"
#include "HttpError.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Location.hpp"
#include "Logger.hpp"
#include "MimeTypes.hpp"
#include "ProcessState.hpp"
#include "RedirectStrategy.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "Server.hpp"
#include "ServerManager.hpp"
#include "UploadStrategy.hpp"
#include <exception>
#include <map>
#include <ostream>
#include <pthread.h>
#include <string>
#include <sys/wait.h>

/**
 This class is in charge of choosing the right strategy depending on the request.
 */
template < class ServerClass, class RouteClass >
ResponseBuildState< ServerClass, RouteClass >::ResponseBuildState(int socket, ClientRequest *request, int port) :
    ProcessState(socket),
    _request(request),
    _server(0),
    _strategy(0),
    _recovery(false),
    _code(OK) {
    if (!_request) {
        error.log() << "ResponseBuildState: Trying to build response without request, sending " << InternalServerError
                    << std::endl;
        _recovery = true;
        _code     = InternalServerError;
    } else
        _code = _request->get_status();
#ifndef TESTING
    if (_request->gateway_checks(port)) {
        ServerClass &tmp = ServerManager::getInstance()->getServer(_request->get_header().at("Host"), port);
        _server          = &tmp;
    }
    if (!_server) {
        error.log() << "ResponseBuildState: Trying to build non recovery response without server." << std::endl;
        _recovery = true;
        _code     = InternalServerError;
    }
#else
    (void) port;
#endif
}

template < class ServerClass, class RouteClass >
ResponseBuildState< ServerClass, RouteClass >::ResponseBuildState(int socket, HttpCode code, ServerClass *server) :
    ProcessState(socket),
    _request(0),
    _server(server),
    _strategy(0),
    _recovery(true),
    _code(code) {
    if (!isError(code)) {
        error.log() << "ResponseBuildState: Trying to build recovery response using '" << code
                    << "' code, wich is not an error code. Sending " << InternalServerError << "." << std::endl;
        _code = InternalServerError;
    }
    debug.log() << "Initializing recovery response for " << code << std::endl;
    if (!_server)
        debug.log() << "No server provided for recovery, auto-generating page by default." << std::endl;
}

template < class ServerClass, class RouteClass >
ResponseBuildState< ServerClass, RouteClass >::~ResponseBuildState() {
    if (_strategy)
        delete _strategy;
}

template < class ServerClass, class RouteClass >
t_state ResponseBuildState< ServerClass, RouteClass >::process() {
    if (_recovery) {
        init_strategy(_code);
        _strategy->build_response();
        return _strategy->is_built() ? (_state = ready) : (_state = waiting);
    }
    try { // handling of bad_alloc out of scope
        try {
            if (!_strategy)
                init_strategy();
            _strategy->build_response();
        } catch (HttpError &e) {
            _code = e.get_code(); // max 3 try
            if (_strategy) {
                delete _strategy;
                _strategy = 0;
            }
            init_strategy();
            _strategy->build_response();
        }
    } catch (HttpError &e) {
        init_strategy(e.get_code()); // Should never happen (or if someone delete an error file while being read)
        _strategy->build_response();
    }
    return _strategy->is_built() ? (_state = ready) : (_state = waiting);
}

template < class ServerClass, class RouteClass >
HttpCode ResponseBuildState< ServerClass, RouteClass >::get_status() const {
    return _code;
}

// DEPRECATED AS THERE MAY BE NO REQUEST
// template < class ServerClass, class RouteClass >
// ClientRequest *ResponseBuildState< ServerClass, RouteClass >::get_request() {
//     if (!_request)
//         debug.log() << "ResponseBuildState get_request() called but object has no request." << std::endl;
//     return _request;
// }

// DEPRECATED AS THERE MAY BE NO REQUEST
template < class ServerClass, class RouteClass >
ResponseBuildingStrategy *ResponseBuildState< ServerClass, RouteClass >::get_response_strategy() {
    ResponseBuildingStrategy *ret = _strategy;
    if (_strategy == 0)
        error.log() << "ResponseBuildState: Getting a non existing ResponseBuildingStrategy." << std::endl;
    if (_state != ready)
        warn.log() << "ResponseBuildState: getting ResponseBuildingStrategy that is not totally generated. As a result,"
                      " it will not be removed from ResponseBuildState object and may be deleted at its destruction."
                   << std::endl;
    else
        _strategy = 0;
    return ret;
}

/**
  This function build the right strategy for a given ClientRequest.
  */
template < class ServerClass, class RouteClass > // OPTI: refactor this this is sooo bad
void ResponseBuildState< ServerClass, RouteClass >::init_strategy() {
    if (_recovery || isError(_code)) {
        if (!_server) {
            debug.log() << "Sending " << _code << " with generated page" << std::endl;
            _strategy = new ErrorStrategy(_code); // page not found
            return;
        }
        const std::map< HttpCode, std::string > &error_pages = _server->getErrorPages();

        std::map< HttpCode, std::string >::const_iterator it = error_pages.find(_code);

        if (it == error_pages.end()) {
            debug.log() << "Sending " << _code << " with generated page" << std::endl;
            _strategy = new ErrorStrategy(_code); // page not found
        } else {                                  // page found
            debug.log() << "Sending " << _code << " with file " << it->second << std::endl;
            _strategy = new GetFileStrategy(mime_types, it->second, _code);
        }
        return;
    }
    if (!_server) {
        error.log() << "Trying to instantiate non error response without server, sending " << InternalServerError
                    << std::endl;
        throw HttpError(InternalServerError); // throw is valid here and will be cached
    }

    Location< ServerClass, RouteClass > location(_request->get_target(), *_server);

    if (location.is_redirect()) {
        debug.log() << "Choosing RedirectStrategy" << std::endl;
        _strategy = new RedirectStrategy(location.get_path(), _request->get_query_string(), location.get_status_code());
    } else if (location.is_cgi()) {
        debug.log() << "Choosing CGIStrategy" << std::endl;
        _strategy = new CGIStrategy(location.get_path(), _request, location.get_path_info(), location.get_cgi_path());
    } else if (_request->get_method() == GET) {
        if (!location.is_get()) {
            info.log() << "ResponseBuildState: GET method is not allowed in route '" << location.get_route()
                       << "', sending " << MethodNotAllowed << std::endl;
            throw HttpError(MethodNotAllowed);
        }
        if (location.is_file()) {
            debug.log() << "Choosing GetFileStrategy" << std::endl;
            _strategy = new GetFileStrategy(mime_types, location.get_path());
        } else if (location.has_autoindex()) {
            debug.log() << "Choosing GetIndexStrategy" << std::endl;
            _strategy = new GetIndexStrategy(location.get_path(), _request->get_target());
        } else {
            info.log() << "ResponseBuildState: '" << _request->get_target() << "' is not a file, sending " << Forbidden
                       << std::endl;
            throw HttpError(Forbidden);
        }
    } else if (_request->get_method() == POST) {
        if (!location.is_post()) {
            info.log() << "ResponseBuildState: POST method is not allowed in route '" << location.get_route()
                       << "', sending " << MethodNotAllowed << std::endl;
            throw HttpError(MethodNotAllowed);
        }
        debug.log() << "Choosing UploadStrategy" << std::endl;
        _strategy = new UploadStrategy< ServerClass, RouteClass >(
            *_request, location.get_upload_path(), *_server, location.is_diff()
        );
    } else if (_request->get_method() == DELETE) {
        if (!location.is_delete()) {
            info.log() << "ResponseBuildState: DELETE method is not allowed in route '" << location.get_route()
                       << "', sending " << MethodNotAllowed << std::endl;
            throw HttpError(MethodNotAllowed);
        }
        debug.log() << "Choosing DeleteStrategy" << std::endl;
        _strategy = new DeleteStrategy(location.get_path());
    }
    if (!_strategy) {
        error.log() << "ResponseBuildState: no strategy selected for a given request. Throwing " << InternalServerError
                    << std::endl;
        throw HttpError(InternalServerError);
    }
} // 27 not that bad - 27 wth are u even talking about, more like 53 + 21 = 74 !!%#@

/**
  Build error strategy for a given http code.
  */
template < class ServerClass, class RouteClass >
void ResponseBuildState< ServerClass, RouteClass >::init_strategy(HttpCode code) {
    if (!isError(code))
        warn.log() << "Generating error for non error code " << code << std::endl;
    try {
        debug.log() << "Generating error page for " << code << std::endl;
        _strategy = new ErrorStrategy(code);
    } catch (std::exception &e) {
        throw e; // do not try to send response after this, only close connection.
        // WARN: check not to loop in recovery there.
    }
}

template < class ServerClass, class RouteClass >
void ResponseBuildState< ServerClass, RouteClass >::save_mem() {
    if (_request)
        _request->save_mem();
    if (_strategy)
        _strategy->save_mem();
}

template class ResponseBuildState<>; // force compilation for this template (defaults)

#ifdef TESTING
# include "FakeRoute.hpp"
# include "FakeServer.hpp"
template class ResponseBuildState< FakeServer, FakeRoute >; // force compilation for test templates
#endif
