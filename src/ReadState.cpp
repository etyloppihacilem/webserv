/* #############################################################################

               """          ReadState.cpp
        -\-    _|__
         |\___/  . \        Created on 01 Jun. 2024 at 14:23
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ReadState.hpp"
#include "ClientRequest.hpp"
#include "HttpStatusCodes.hpp"
#include "HttpUtils.hpp"
#include "Logger.hpp"
#include "ProcessState.hpp"
#include "StringUtils.hpp"
#include "todo.hpp"
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <ostream>
#include <string>
#include <unistd.h>

ReadState::ReadState(int socket, int port, const std::string &ip) :
    ProcessState(socket),
    _buffer(),
    _request(0),
    _parse_state(rs_line),
    _port(port),
    _ip(ip) {
    info.log() << "Parsing incoming request." << std::endl;
}

ReadState::~ReadState() {
    if (_request)
        delete _request;
}

/**
  Function that read from socket and processes the buffer.
  */
t_state ReadState::process() {
    if (_state == ready || _state == s_error)
        return _state;

    char buffer[BUFFER_SIZE + 1] = { 0 };

    int a = 0;
    if (_state == waiting) {

        if ((a = read(_socket, buffer, BUFFER_SIZE)) < 0) {
            error.log() << "Reading into socket " << _socket << " resulted in error: " << strerror(errno)
                        << ", closing connection." << std::endl;
            return _state = s_error;
        } else if (a == 0) {
            warn.log() << "Reading nothing into socket " << _socket << ", closing connection." << std::endl;
            return _state = s_error;
        }
    }
    return process_buffer(buffer, a);
}

/**
  Function that processes the buffer without reading from socket.

  Called by process().
  */
t_state ReadState::process_buffer(char *buffer, int size_read) {
    size_t eol; // end of line
    _buffer.append(buffer, size_read);
    sanitize_HTTP_string(_buffer, 0);
    if (_state == waiting) {
        if (_request == 0) {
            _request = new ClientRequest(_socket, _ip);
            debug.log() << "Parsing request line." << std::endl;
        }
        // length check"
        while (_parse_state != body && (eol = _buffer.find("\n")) != _buffer.npos) {
            if (_parse_state == rs_line && eol == 0) {
                _buffer = _buffer.substr(1, _buffer.length() - 1); // to discard leading lines of request
                sanitize_HTTP_string(_buffer, 0);
            } else if (_parse_state == rs_line) {
                if (!_request->parse_request_line(_buffer))
                    return _state = ready; // ready to return error
                debug.log() << "Parsing headers." << std::endl;
                _parse_state = headers; // next state
            } else if (_parse_state == headers) {
                if (_request->parse_headers(_buffer)) {
                    if (isError(_request->get_status()))
                        return _state = ready;
                    debug.log() << "Processing gateway checks." << std::endl;
                    if (!_request->gateway_checks(_port))
                        return _state = ready; // error
                    debug.log() << "Parsing body if any." << std::endl;
                    _parse_state = body;
                }
            }
        }
        if (_buffer.length() > MAX_REQUEST_LINE) {
            _buffer = "";
            shrink_to_fit(_buffer); // because it is large
            info.log() << "MAX_REQUEST_LINE was reached while parsing "
                       << (_parse_state == rs_line ? "request_line" : "headers") << ", sending "
                       << (_parse_state == rs_line ? URITooLong : RequestHeaderFieldsTooLarge) << std::endl;
            return_error(_parse_state == rs_line ? URITooLong : RequestHeaderFieldsTooLarge);
        }
        if (_parse_state == body) {
            _request->init_body(_buffer);
            _parse_state = finished;
            _state       = ready;
            info.log() << "Just parsed " << _request->get_method() << " " << _request->get_target() << std::endl;
        }
    }
    return _state;
}

t_state ReadState::return_error(HttpCode code) {
    if (_request)
        delete _request;
    _request      = new ClientRequest(_socket, code, _port);
    return _state = ready;
}

/**
  Returns pointer on generated request
  */
ClientRequest *ReadState::get_client_request() {
    ClientRequest *ret = _request;
    if (_request == 0)
        error.log() << "Getting a non existing ClientRequest." << std::endl;
    if (_state != ready)
        warn.log() << "ReadState: getting ClientRequest that is not totally generated. As a result, it will not be "
                      "removed from ReadState object and may be deleted at its destruction."
                   << std::endl;
    else {
        _request = 0;
        _state   = waiting;
    }
    return ret;
}

void ReadState::save_mem() {
    shrink_to_fit(_buffer);
    if (_request)
        _request->save_mem();
}
