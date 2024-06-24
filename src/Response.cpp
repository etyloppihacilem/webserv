/* #############################################################################

               """          Response.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:47
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Response.hpp"
#include "HttpStatusCodes.hpp"
#include "todo.hpp"
#include "BodyWriterChunk.hpp"
#include "BodyWriterLength.hpp"
#include "ResponseBuildingStrategy.hpp"
#include <new>
#include <sstream>

Response::Response():
    _code   (OK),
    _header (),
    _body   (0) {}

Response::~Response() {
    if (_body)
        delete _body;
}

/**
  Set a body that is already built. Remember that Response will delete any saved body at destruction.
  */
void Response::set_body(BodyWriter *body) {
    _body = body;
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
  Generate response.

  I do not know what it really means yet. Mayby just the headers.
  */
std::string Response::build_response() {
    std::string res = "";

    // TODO construire la Response
    // [x] premiere ligne
    // [x] headers
    // [ ] body
    //      [ ] length
    //      [ ] chunked
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

    line << "HTTP/1.1 " << _code << " " << status_string(_code) << "\r\n";
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
        _body                           = new BodyWriterChunk(strategy);
        _header["Transfer-Encoding"]    = "chunk";
    } else {
        try {
            _body = new BodyWriterLength(strategy);
        } catch (std::bad_alloc &e) {
            _body                           = new BodyWriterChunk(strategy);
            _header["Transfer-Encoding"]    = "chunk";
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
