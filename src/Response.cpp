/* #############################################################################

               """          Response.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:47
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyWriter.hpp"
#include "BodyWriterChunk.hpp"
#include "BodyWriterLength.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "Response.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <new>
#include <ostream>
#include <sstream>
#include <string>

Response::Response():
    _code   (OK),
    _header (),
    _body   (0) {
    add_header("Server", "webserv"); // adding server name
}

// TODO:connection header is not there yet !!!!

Response::~Response() {
    if (_body)
        delete _body;
}

/**
  Add header to Response _header
  */
void Response::add_header(const std::string &field, const std::string &value) {
    if (value == "") {
        _header.erase(field);
        return;
    }
    if (_header.find(field) != _header.end())
        _header[field] += ", " + value;
    else
        _header[field] = value;
}

/**
  Sets response code.
  */
void Response::set_code(const HttpCode &code) {
    _code = code;
}

/**
  Returns response code.
  */
HttpCode Response::get_code() const {
    return _code;
}

/**
  Returns body.
  */
BodyWriter *Response::get_body() {
    if (!_body)
        warn.log() << "Getting response body, but it does not exist." << std::endl;
    return _body;
}

/**
  Returns true if BodyWriter exists
  */
bool Response::have_body() {
    return _body != 0;
}

/**
  Generate response.

  I do not know what it really means yet. Mayby just the headers.
  */
std::string Response::build_response() {
    std::string res = "";

    res = generate_status_line();
    res += generate_header();
    return res;
}

/**
  Generate status line with the following syntax :
  ```
  HTTP/1.1 CODE STATUS_MESSAGE\r\n
  ```
  With CODE being the status code and STATUS_MESSAGE the default status string.
  */
std::string Response::generate_status_line() const {
    std::stringstream line;

    line << "HTTP/1.1 " << _code << "\r\n"; // overload with HttpStatusCodes
    return line.str();
}

/**
  Generate headers for response. The syntax is the following :
  ```
  NAME: VALUE\r\n
  ```
  */
std::string Response::generate_header() const {
    std::stringstream headers;

    for (mapit i = _header.begin(); i != _header.end(); i++) {
        headers << i->first << ": " << i->second << "\r\n";
    }
    return headers.str();
}

/**
  Delete _body and sets it to 0
  */
void Response::clean_body() {
    if (_body)
        delete _body;
    _body = 0;
}

/**
  Builds body from a ResponseBuildingStrategy object.

  BodyWriter object uses the ResponseBuildingStrategy.fill_buffer() method to send body chunked or by length.
  */
void Response::set_body(ResponseBuildingStrategy &strategy) {
    clean_body();
    if (strategy.get_estimated_size() > MAX_BODY_BUFFER) {
        _body = new BodyWriterChunk(strategy);
        _header["Transfer-Encoding"] = "chunk";
    } else {
        try {
            _body = new BodyWriterLength(strategy);
        } catch (std::bad_alloc &e) {
            _body = new BodyWriterChunk(strategy);
            _header["Transfer-Encoding"] = "chunk";
            return;
        }

        std::stringstream st;

        st << _body->length();
        _header["Content-Length"] = st.str();
    }
}

void Response::save_mem() {
    _body->save_mem();
}
