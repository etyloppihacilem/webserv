/* #############################################################################

               """          Response.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:47
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Response.hpp"
#include "BodyWriter.hpp"
#include "HttpStatusCodes.hpp"
#include "todo.hpp"
#include "BodyWriterChunk.hpp"
#include "BodyWriterLength.hpp"
#include "ResponseBuildingStrategy.hpp"
#include <sstream>
#include <string>

Response::Response():
    _code   (OK),
    _header (),
    _body   (0) {}

Response::~Response() {
    if (_body)
        delete _body;
}

void Response::set_body(BodyWriter *body) {
    _body = body;
}

void Response::add_header(const std::string &field, const std::string &value) {
    _header[field] = value;
}

void Response::set_code(const HttpCode &code) {
    _code = code;
}

HttpCode Response::get_code() const {
    return (_code);
}

std::string Response::build_response() {
    std::string res = "";

    // TODO construire la Response
    // [x] premiere ligne
    // [x] headers
    // [ ] body
    //      [ ] length
    //      [ ] chunked
    return (res);
}

std::string Response::generate_status_line() const {
    std::stringstream line;

    line << "HTTP/1.1 " << _code << " " << status_string(_code) << "\r\n";
    return (line.str());
}

std::string Response::generate_header() const {
    std::stringstream headers;

    for (mapit i = _header.begin(); i != _header.end(); i++) {
        headers << i->first << ": " << i->second << "\r\n";
    }
    return (headers.str());
}

void Response::clean_body() {
    if (_body)
        delete _body;
    _body = 0;
}

void Response::set_body(ResponseBuildingStrategy *strategy) {
    clean_body();
    if (strategy->get_estimated_size() > MAX_BODY_BUFFER)
        _body = new BodyWriterChunk(strategy);
    else
        _body = new BodyWriterLength(strategy);
}
