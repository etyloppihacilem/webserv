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
#include "CGIStrategy.hpp"
#include "CGIWriter.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ProcessState.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <cstddef>
#include <ctime>
#include <new>
#include <ostream>
#include <sstream>
#include <string>

Response::Response() : _code(OK), _header(), _body(0), _state(rs_line), _is_head(false) {
    add_header("Server", SERVER_SOFTWARE); // adding server name, this is not MUST but SHOULD in RFC
    _header["Connection"]  = "keep-alive";
    _last_modified.tv_sec  = 0;
    _last_modified.tv_nsec = 0;
}

// Logic of connection header be :
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
        _header[field] += ", " + value + (*_header[field].rbegin() == ';' ? "" : ";");
    else
        _header[field] = value;
}

/**
  Sets response code.
  */
void Response::set_code(HttpCode code) {
    _code = code;
    if (_is_head && _code == OK)
        _code = NoContent; // in ubuntu_tester excpected 204.
    if (isError(_code) || isRedirection(_code))
        _header["Connection"] = "close";
    else
        _header["Connection"] = "keep-alive";
}

/**
  To parse CGI code
  */
void Response::set_code(std::string code) {
    size_t end = code.find_first_of(" \t");
    if (end != code.npos)
        code.resize(end);
    std::stringstream st;
    st << code;
    int tmp;
    if (!(st >> tmp)) {
        error.log() << "Could not parse response code from string '" << code << "', sending " << InternalServerError << std::endl;
        _code = InternalServerError;
    }
    if (status_string(tmp) != "")
        _code = static_cast< HttpCode >(tmp);
    else {
        error.log() << tmp << " ('" << code << "') is not a valid HttpCode, sending " << InternalServerError << std::endl;
        _code = InternalServerError;
    }
    if (isError(_code) || isRedirection(_code))
        _header["Connection"] = "close";
    else
        _header["Connection"] = "keep-alive";
}

void Response::date() {
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    char strNow[TIME_LENGTH] = { 0 };
    strftime(strNow, TIME_LENGTH, "%a, %d %b %Y %H:%M:%S %Z", gmtime(&now.tv_sec));
    _header["Date"] = strNow;
    if (!isError(_code) && (_last_modified.tv_nsec != 0 || _last_modified.tv_sec != 0)) {
        now.tv_sec += 604800;
        strftime(strNow, TIME_LENGTH, "%a, %d %b %Y %H:%M:%S %Z", gmtime(&now.tv_sec));
        _header["Expire"] = strNow;
        strftime(strNow, TIME_LENGTH, "%a, %d %b %Y %H:%M:%S %Z", gmtime(&_last_modified.tv_sec));
        _header["Last-Modified"] = strNow;

        _header["Cache-Control"] = "private, max-age=604800;";
    }
    if (isError(_code))
        _header["Cache-Control"] = "no-store";
}

void Response::set_last_modified(const struct timespec &time) {
    _last_modified.tv_sec  = time.tv_sec;
    _last_modified.tv_nsec = time.tv_nsec;
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
    while (buffer.length() < size && _state != finished) {
        if (_state == rs_line) {
            if (_body->init_todo()) {
                CGIWriter *tmp;
                if ((tmp = dynamic_cast< CGIWriter * >(_body)) != 0) {
                    if (tmp->init()) {
                        debug.log() << "CGIWriter init not done yet." << std::endl;
                        break;
                    }
                } else {
                    warn.log() << "No init on no CGIWriter BodyWriter" << std::endl;
                }
            }
            debug.log() << "Generating status line" << std::endl;
            date(); // updating date headers
            buffer = generate_status_line();
            _state = headers;
        } else if (_state == headers) {
            if (_header.find("Content-Length") == _header.end()) { // here ?
                debug.log() << "CGI did not defined length, sending body in chunked format." << std::endl;
                _header["Transfer-Encoding"] = "chunked";
            }
            debug.log() << "Generating headers" << std::endl;
            buffer += generate_header();
            buffer += "\r\n"; // to end headers
            if (_body) {
                if (_is_head) {
                    debug.log() << "HEAD request, not sending body." << std::endl;
                    _state = finished;
                }
                debug.log() << "Generating body" << std::endl;
                _state = body;
            } else {
                _state = finished;
                debug.log() << "Response generated" << std::endl;
            }
        } else if (_state == body) {
            if (!_body->is_done())
                buffer += _body->generate(BUFFER_SIZE);
            if (_body->is_done()) { // no else bc need to check after first if
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

    line << "HTTP/1.1 " << _code << "\r\n"; // overload with HttpStatusCodes so message is automatically there
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
    if (_header.find("Content-Length") != _header.end())
        _header.erase("Content-Length");
    if (_header.find("Transfer-Encoding") != _header.end())
        _header.erase("Transfer-Encoding");
    if (_body)
        delete _body;
    _body = 0;
}

void Response::set_body(const std::string &body_content, std::string content_type) {
    std::stringstream st;
    clean_body();
    debug.log() << "Sending manual body format: length" << std::endl;
    _body = new BodyWriterLength(body_content);
    st << _body->length();
    debug.log() << "Response sending format: length." << std::endl;
    _header["Content-Length"] = st.str();
    debug.log() << "Manual body Content-Type: " << content_type << std::endl;
    _header["Content-Type"] = content_type;
}

void Response::set_cgi(ResponseBuildingStrategy *strategy, CGIWriter &writer) {
    if (!strategy) {
        error.log() << "Trying to set response CGI without strategy. Aborting." << std::endl;
        return;
    }
    clean_body();
    debug.log() << "Setting response with CGI." << std::endl;
    _body = dynamic_cast< BodyWriter * >(&writer);
    // _body->generate(); // TODO: not there // this will initiate response headers EXCEPT connection !!
    // if (_header.find("Content-Length") == _header.end()) {
    //     debug.log() << "CGI did not defined length, sending body in chunked format." << std::endl;
    //     _header["Transfer-Encoding"] = "chunked";
    // } else {
    //     CGIStrategy *tmp = dynamic_cast< CGIStrategy * >(strategy);
    //     if (tmp)
    //         tmp->set_length(true);
    //     else
    //         error.log() << "CGI is set with no CGIStrategy Strategy." << std::endl;
    // }
}

/**
  Builds body from a ResponseBuildingStrategy object.

  BodyWriter object uses the ResponseBuildingStrategy.fill_buffer() method to send body chunked or by length.
  */
void Response::set_body(ResponseBuildingStrategy *strategy) {
    if (!strategy) {
        error.log() << "Trying to set response body without strategy. Aborting." << std::endl;
        return;
    }
    clean_body();
    if (strategy->get_estimated_size() > MAX_BODY_BUFFER) {
        _body = new BodyWriterChunk(*strategy);
        debug.log() << "Response sending format: chunked." << std::endl;
        _header["Transfer-Encoding"] = "chunked";
    } else {
        std::stringstream st;
        try {
            _body = new BodyWriterLength(*strategy);
            st << _body->length();
            debug.log() << "Response sending format: length." << std::endl;
        } catch (std::bad_alloc &e) {
            _body                        = new BodyWriterChunk(*strategy);
            _header["Transfer-Encoding"] = "chunked";
            warn.log() << "Response sending format: chunked (bad_alloc)." << std::endl;
            return;
        }
        _header["Content-Length"] = st.str();
    }
}

void Response::set_head(bool is_head) {
    _is_head = is_head;
    if (_is_head && _code == OK)
        _code = NoContent; // in ubuntu_tester excpected 204.
}

void Response::save_mem() {
    _body->save_mem();
}
