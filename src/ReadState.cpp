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
#include "Logger.hpp"
#include "ProcessState.hpp"
#include "StringUtils.hpp"
#include "todo.hpp"
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <ostream>
#include <ostream>
#include <unistd.h>

ReadState::ReadState(int fd):
    ProcessState(fd),
    _buffer     (),
    _in_progress(0) {}

ReadState::~ReadState() {
    if (_in_progress)
        delete _in_progress;
}

/**
  Function that read from fd and processes the buffer.
  */
t_state ReadState::process() {
    if (_state == ready || _state == s_error)
        return _state;

    char buffer[BUFFER_SIZE + 1] = {
        0
    };

    if (_state == waiting) {
        int a;

        if ((a = read(_fd, buffer, BUFFER_SIZE)) < 0)
            error.log() << "Reading into socket " << _fd << " resulted in error: " << strerror(errno) << std::endl;
        else if (a == 0) {
            // this should not happen.
            warn.log()  << "Reading nothing into socket " << _fd << ", closing connection with " << BadRequest
                        << std::endl;
            return return_error();
        }
    }
    return process_buffer(buffer);
}

/**
  Function that processes the buffer without reading from fd.

  Called by process().
  */
t_state ReadState::process_buffer(char *buffer) {
    _buffer += buffer;
    if (_state == waiting) {
        // getting stuff that needs to be detected as header.
        // if _buffer does not start with a request line, everything will be discarded
        // until request line is found.
        if (_in_progress) {
            delete _in_progress;
            _in_progress = 0;
        }

        // TODO: verifier la longueur du buffer ?
        size_t  end = _buffer.find("\r\n\r\n", 0);
        size_t  eol = _buffer.find("\r\n");

        if (end == _buffer.npos)
            return _state;
        if (eol >= end) {
            _buffer = _buffer.substr(end + 4, _buffer.length() - (end + 4));
            return return_error();
        }
        // if (end > MAX_HEADER) // TODO:est-ce que le max header existe ??
        //                          _buffer = "" et il faut repondre par une erreur
        _in_progress = new ClientRequest(_fd);
        if (!_in_progress->parse_header(_buffer))
            return _state = ready; // TODO:close connection after error response is sent.

        //_buffer = _buffer.substr(0, end);  // wtf is this what about the body ????
        _buffer = _buffer.substr(end + 4, _buffer.length() - (end + 4));
        _in_progress->init_body(_buffer);
        _state  = ready;
    }
    return _state;
}

t_state ReadState::return_error() {
    if (_in_progress)
        delete _in_progress;
    _in_progress    = new ClientRequest(_fd, BadRequest, 80);   // TODO: trouver si le port est necessaire et
    return _state   = ready;                                    // mettre le bon le cas echeant.
}

/**
  Function to delete message once the server is done generating response.

  Message should not be deleted anywhere else.
  */
void ReadState::done_client_request() {
    if (_in_progress)
        delete _in_progress;
    _in_progress    = 0;
    _state          = waiting;
}

/**
  Returns pointer on generated (or generating message)
  */
ClientRequest *ReadState::get_client_request() {
    return _in_progress;
}

void ReadState::save_mem() {
    shrink_to_fit(_buffer);
    if (_in_progress)
        _in_progress->save_mem();
}
