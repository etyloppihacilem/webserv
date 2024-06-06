/* #############################################################################

               """          ReadState.cpp
        -\-    _|__
         |\___/  . \        Created on 01 Jun. 2024 at 14:23
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ReadState.hpp"
#include "HttpMethods.hpp"
#include "ClientRequest.hpp"
#include "todo.hpp"
#include <cstddef>
#include <strings.h>
#include <unistd.h>

ReadState::ReadState(int fd):
    _fd         (fd),
    _state      (waiting),
    _buffer     (),
    _ready      (0),
    _in_progress(0) {}

ReadState::~ReadState() {
    if (_ready)
        delete _ready;
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
        return (found);
    return (ret);
}

void ReadState::process() {
    size_t  bytes_read;
    char    buffer[BUFFER_SIZE + 1];

    if (_state == waiting) {
        bzero(buffer, sizeof(buffer));
        bytes_read = read(_fd, buffer, BUFFER_SIZE);
    }
    process_buffer(buffer);
}

t_state ReadState::process_buffer(char *buffer) {
    _buffer += buffer;
    if (_state == waiting_body) {
        // body input on message in progress
        if (!_in_progress)
            return (_state = waiting); // this should not happen
    }
    if (_state == waiting) {
        // getting stuff that needs to be detected as header.
        // if _buffer does not start with a request line, everything will be discarded
        // until request line is found.
        if (_in_progress) {
            delete _in_progress;
            _in_progress = 0;
        }

        size_t begin = find_method();

        if (begin == _buffer.npos)
            _buffer = "";
        return (_state);
        if (begin != 0)
            _buffer = _buffer.substr(begin, _buffer.length() - begin);

        // verifier la longueur du buffer
        size_t end = _buffer.find("\r\n\r\n", 0);

        if (end == _buffer.npos)
            return (_state);
        // if (end - begin > MAX_HEADER) // est-ce que le max header existe ??
        // _buffer = "" et il faut repondre par une erreur
        _in_progress = new ClientRequest;
        if (!_in_progress->parse_header(_buffer))
            return (_state = error);
        _buffer = _buffer.substr(0, end);
        if (_in_progress->init_body(_buffer, _fd)) {
            _state          = waiting_body;
            _ready          = _in_progress;
            _in_progress    = 0;
        } else {
            _state          = ready;
            _ready          = _in_progress;
            _in_progress    = 0;
        }
    }
    return (_state);
}
