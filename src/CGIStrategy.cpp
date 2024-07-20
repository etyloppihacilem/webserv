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
#include "ResponseBuildingStrategy.hpp"
#include "StringUtils.hpp"
#include "todo.hpp"
#include <cstddef>
#include <map>
#include <sstream>
#include <string>

CGIStrategy::CGIStrategy(const std::string &location, ClientRequest *request):
    ResponseBuildingStrategy(),
    _location               (location),
    _request                (request) {}

CGIStrategy::~CGIStrategy() {}

bool CGIStrategy::build_response() {
    Body    *body   = 0;
    size_t  size    = 0;

    if (_request->have_body()) { // WARN: check for infinite loop risk. Test with both body types.
        body = _request->get_body();
        if (dynamic_cast<BodyChunk *>(body)) {  // if body is chunk
            while (body->is_done())             // retrieving whole body.
                body->get();
        }
        size = body->length();
    }

    std::map<std::string, std::string> env;

    env["CONTENT_TYPE"]         = "";
    if (size) {
        std::stringstream st;

        st << size;
        st >> env["CONTENT_LENGTH"];
    }
    env["GATEWAY_INTERFACE"]    = "CGI/1.1";
    env["PATH_INFO"]            = ""; // TODO: trouver PATH_INFO /test/cgi.bin/monscript.cgi/path_info
    //                                                                                      ^^^^^^^^^^
    env["PATH_TRANSLATED"]      = ""; // TODO: apres la conversion en physique
    env["QUERY_STRING"]         = _request->get_query_string();
    env["REMOTE_ADDR"]          = "";
    env["REQUEST_METHOD"]       = method_string(_request->get_method());
    env["SCRIPT_NAME"]          = "";
    env["SERVER_NAME"]          = ""; // TODO: trouver le server name
    env["SERVER_PORT"]          = ""; // TODO: trouver le port
    env["SERVER_PROTOCOL"]      = "HTTP/1.1";
    env["SERVER_SOFTWAR"]       = SERVER_SOFTWARE;
    return _built;
}

bool CGIStrategy::fill_buffer(std::string &buffer, size_t size) {
    (void) buffer;
    (void) size;
    return _done;
}

void CGIStrategy::save_mem() {
    shrink_to_fit(_buffer);
    _response.save_mem();
}
