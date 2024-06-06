/* #############################################################################

               """          Response.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:47
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Response.hpp"
#include "HttpStatusCodes.hpp"
#include <string>

Response::Response():
    _code   (OK),
    _header (),
    _body   () {}

Response::~Response() {}

void Response::set_body(std::string body) {
    _body = body;
}

void Response::add_header(std::string field, std::string value) {
    _header[field] = value;
}

void Response::set_code(HttpCode code) {
    _code = code;
}

HttpCode Response::get_code() const {
    return (_code);
}

std::string Response::build_response() {
    std::string res = "";

    // TODO construire la Response
    // - premiere ligne
    // - headers
    // - body
    return (res);
}
