/* #############################################################################

               """          Response.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:47
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Response.hpp"
#include "BodyWriter.hpp"
#include "BodyWriterChunk.hpp"
#include "BodyWriterLength.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ProcessState.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <cstddef>
#include <new>
#include <ostream>
#include <sstream>
#include <string>

Response::Response() : _code(OK), _header(), _body(0), _state(rs_line) {
    add_header("Server", SERVER_SOFTWARE); // adding server name, this is not MUST but SHOULD in RFC
}

// TODO:connection header is not there yet !!!!
// Logic should be :
//  - in case of error (error code 4xx or 5xx) : connection close;
//  - else : connection keep-alive
// check how to close timeout connection
Response::~Response() {
    if (_body) {
        debug.log() << "Response deletes BodyWriter" << std::endl;
        delete _body;
    }
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
bool Response::build_response(std::string &buffer, size_t size) {
    if (_state == finished) {
        warn.log() << "Response already done generating" << std::endl;
        return true;
    }
    while (buffer.length() < size) {
        if (_state == rs_line) {
            debug.log() << "Generating status line" << std::endl;
            buffer = generate_status_line();
            _state = headers;
        } else if (_state == headers) {
            debug.log() << "Generating headers" << std::endl;
            buffer += generate_header();
            if (_body) {
                debug.log() << "Generating body" << std::endl;
                _state = body;
            } else {
                _state = finished;
                debug.log() << "Response generated" << std::endl;
            }
        } else if (_state == body) {
            if (!_body->is_done())
                buffer += _body->generate();
            if (_body->is_done()) {
                debug.log() << "Body is done generating" << std::endl;
                _state = finished;
            }
        }
    }
    return _state == finished;
}

bool Response::is_done() const {
    return _state == finished;
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

    for (mapit i = _header.begin(); i != _header.end(); i++)
        headers << i->first << ": " << i->second << "\r\n";
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
void Response::set_body(ResponseBuildingStrategy *strategy) {
    if (!strategy)
        return;
    clean_body();
    if (strategy->get_estimated_size() > MAX_BODY_BUFFER) {
        _body = new BodyWriterChunk(*strategy);
        debug.log() << "Response sending format : chunk." << std::endl;
        _header["Transfer-Encoding"] = "chunk";
    } else {
        std::stringstream st;
        try {
            _body = new BodyWriterLength(*strategy);
            st << _body->length();
            debug.log() << "Response sending format : chunk." << std::endl;
        } catch (std::bad_alloc &e) {
            _body                        = new BodyWriterChunk(*strategy);
            _header["Transfer-Encoding"] = "chunk";
            warn.log() << "Response sending format : chunk (bad_alloc)." << std::endl;
            return;
        }
        _header["Content-Length"] = st.str();
    }
}

void Response::save_mem() {
    _body->save_mem();
}
