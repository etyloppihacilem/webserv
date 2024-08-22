/* #############################################################################

               """          ReadState.cpp
        -\-    _|__
         |\___/  . \        Created on 01 Jun. 2024 at 14:23
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ReadState.hpp"
#include "ClientRequest.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ProcessState.hpp"
#include "StringUtils.hpp"
#include "todo.hpp"
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <ostream>
#include <unistd.h>

ReadState::ReadState(int socket) : ProcessState(socket), _buffer(), _in_progress(0), _parse_state(request_line) {}

ReadState::~ReadState() {
    if (_in_progress)
        delete _in_progress;
}

/**
  Function that read from socket and processes the buffer.
  */
t_state ReadState::process() {
    if (_state == ready || _state == s_error)
        return _state;

    char buffer[BUFFER_SIZE + 1] = { 0 };

    if (_state == waiting) {
        int a;

        if ((a = read(_socket, buffer, BUFFER_SIZE)) < 0)
            error.log() << "Reading into socket " << _socket << " resulted in error: " << strerror(errno) << std::endl;
        else if (a == 0) {
            // this should not happen.
            warn.log() << "Reading nothing into socket " << _socket << ", closing connection with " << BadRequest
                       << std::endl;
            return return_error();
        }
    }
    return process_buffer(buffer);
}

/**
  Function that processes the buffer without reading from socket.

  Called by process().
  */
t_state ReadState::process_buffer(char *buffer) {
    size_t eol; // end of line
    _buffer += buffer;
    sanitize_HTTP_string(_buffer, 0);
    if (_state == waiting) {
        if (_in_progress == 0)
            _in_progress = new ClientRequest(_socket);
        if ((eol = _buffer.find("\n")) != _buffer.npos) {
            if (_parse_state == request_line) {
                if (!_in_progress->parse_request_line(_buffer))
                    return _state = ready; // ready to return error
                _parse_state = headers;    // next state
            } else if (_parse_state == headers) {
                try {
                    if (_in_progress->parse_headers(_buffer))
                        _parse_state = body;
                } catch (HttpError &e) {
                    return _state = ready; // ready to return on error
                };
            }
        }
        if (_parse_state == body) {
            _in_progress->init_body(_buffer);
            _parse_state = finished;
            _state       = ready;
        }
    }
    return _state;
}

t_state ReadState::return_error() {
    if (_in_progress)
        delete _in_progress;
    _in_progress  = new ClientRequest(_socket, BadRequest, 80); // TODO: trouver si le port est necessaire et
    return _state = ready;                                      // mettre le bon le cas echeant.
}

/**
  Returns pointer on generated request
  */
ClientRequest *ReadState::get_client_request() {
    ClientRequest *ret = _in_progress;
    if (_in_progress == 0)
        error.log() << "Getting a non existing ClientRequest." << std::endl;
    if (_state != ready)
        warn.log() << "ReadState: getting ClientRequest that is not totally generated. As a result, it will not be "
                      "removed from ReadState object and may be deleted at its destruction."
                   << std::endl;
    else {
        _in_progress = 0;
        _state       = waiting;
    }
    return ret;
}

void ReadState::save_mem() {
    shrink_to_fit(_buffer);
    if (_in_progress)
        _in_progress->save_mem();
}
