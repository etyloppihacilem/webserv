/* #############################################################################

               """          CGIStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 26 Jun. 2024 at 11:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "CGIStrategy.hpp"
#include "Body.hpp"
#include "BodyChunk.hpp"
#include "ClientRequest.hpp"
#include "HttpMethods.hpp"
#include "Logger.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "StringUtils.hpp"
#include "todo.hpp"
#include <cstddef>
#include <cstring>
#include <map>
#include <new>
#include <ostream>
#include <sstream>
#include <string.h>
#include <string>
#include <strings.h>

CGIStrategy::CGIStrategy(const std::string &location, ClientRequest *request) :
    ResponseBuildingStrategy(),
    _location(location),
    _request(request) {}

CGIStrategy::~CGIStrategy() {}

bool CGIStrategy::build_response() {
    Body  *body = 0;
    size_t size = 0;

    if (_request->have_body()) { // WARN: check for infinite loop risk. Test with both body types.
        body = _request->get_body();
        if (dynamic_cast<BodyChunk *>(body)) // if body is chunk
            while (body->is_done())          // retrieving whole body. TODO: body cannot be read without a epoll
                body->get();
        size = body->length();
    }

    std::map<std::string, std::string> env;

    env["CONTENT_TYPE"] = "";
    if (size) {
        std::stringstream st;

        st << size;
        st >> env["CONTENT_LENGTH"];
    }
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["PATH_INFO"]         = "";        // trailing things after script name in target
    env["PATH_TRANSLATED"]   = _location; // physical path after translation
    env["QUERY_STRING"]      = _request->get_query_string();
    env["REMOTE_ADDR"]       = "";
    env["REQUEST_METHOD"]    = method_string(_request->get_method());
    env["SCRIPT_NAME"]       = "";
    env["SERVER_NAME"]       = ""; // TODO: trouver le server name
    env["SERVER_PORT"]       = ""; // TODO: trouver le port
    env["SERVER_PROTOCOL"]   = "HTTP/1.1";
    env["SERVER_SOFTWAR"]    = SERVER_SOFTWARE;
    return _built;
}

char **CGIStrategy::generate_env(const std::map<std::string, std::string> &env) const {
    char **ret = 0;

    if (!(ret = new (std::nothrow) char *[env.size() + 1])) {
        error.log() << "Env alloc failed. CGI env will not be generated." << std::endl;
        return 0;
    }
    bzero(ret, env.size() + 1);

    size_t i = 0;

    for (std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); it++) {
        std::string tmp = it->first + "=" + it->second;

        if (!(ret[i] = strdup(tmp.c_str()))) { // bad alloc
            error.log() << "Env alloc partially failed. CGI env will not be generated." << std::endl;
            for (size_t j = 0; j <= env.size(); j++)
                if (ret[j])
                    delete[] ret[j];
            delete[] ret;
            return 0;
        }
    }
    return ret;
}

bool CGIStrategy::fill_buffer(std::string &buffer, size_t size) {
    (void) buffer;
    (void) size;
    return _done;
}

void CGIStrategy::save_mem() {
    _response.save_mem();
}
