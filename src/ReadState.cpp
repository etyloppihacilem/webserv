/* #############################################################################

               """          ReadState.cpp
        -\-    _|__
         |\___/  . \        Created on 01 Jun. 2024 at 14:23
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ReadState.hpp"
#include "todo.hpp"
#include <unistd.h>

ReadState::ReadState(int fd):
    ProcessState(fd),
    _state      (waiting),
    _buffer     (),
    _in_progress(0) {}

ReadState::~ReadState() {
    if (_in_progress)
        delete _in_progress;
}

size_t ReadState::find_method() {
    size_t  ret     = _buffer.find("GET", 0);
    size_t  found   = _buffer.find("POST", 0);

    if (found < ret)
        ret = found;
    found = _buffer.find("DELETE", 0);
    if (found < ret)
        return found;
    return ret;
}

/**
  Function that read from fd and processes the buffer.
  */
bool ReadState::process() {
    if (_state == ready || _state == ready_body)
        return true;

    char buffer[BUFFER_SIZE + 1] = {
        0
    };

    if (_state == waiting) {
        read(_fd, buffer, BUFFER_SIZE);
    }
    process_buffer(buffer);
    return _state == ready || _state == ready_body;
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

        size_t begin = find_method();

        if (begin == _buffer.npos) {
            _buffer = "";
            return _state;
        }

        // verifier la longueur du buffer
        size_t end = _buffer.find("\r\n\r\n", 0);

        if (end == _buffer.npos)
            return _state;
        if (begin != 0)
            _buffer = _buffer.substr(begin, _buffer.length() - begin);
        // if (end - begin > MAX_HEADER) // TODO est-ce que le max header existe ??
        // _buffer = "" et il faut repondre par une erreur
        _in_progress = new ClientRequest;
        if (!_in_progress->parse_header(_buffer))
            return _state = error; // TODO close connection after error response is sent.
        _buffer = _buffer.substr(0, end);
        if (_in_progress->init_body(_buffer, _fd))
            _state = ready_body;
        else
            _state = ready;
    }
    return _state;
}

/**
  Function to delete message once the server is done generating response.

  Message should not be deleted anywhere else.
  */
void ReadState::done_message() {
    if (_in_progress)
        delete _in_progress;
    _in_progress    = 0;
    _state          = waiting;
}

/**
  Returns pointer on generated (or generating message)
  */
ClientRequest *ReadState::get_message() {
    return _in_progress;
}
