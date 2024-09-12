/* #####################################################################################################################

               """          ResponseSendState.cpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 14:31
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ResponseSendState.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ProcessState.hpp"
#include "Response.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "StringUtils.hpp"
#include "todo.hpp"
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <ostream>
#include <unistd.h>

ResponseSendState::ResponseSendState(int socket, ResponseBuildingStrategy *strategy) :
    ProcessState(socket),
    _strategy(strategy),
    _internal_status(unset) {
    if (!strategy) {
        error.log() << "ResponseSendState: no strategy provided, closing connexion" << std::endl;
        _state = s_error;
    }
}

ResponseSendState::~ResponseSendState() {
    if (_strategy) {
        debug.log() << "ResponseSendState deletes Strategy." << std::endl;
        delete _strategy;
    }
}

t_state ResponseSendState::process() {
    if (_state != waiting) {
        warn.log() << "Trying to process ResponseSendState with state '" << (_state == ready ? "ready" : "error") << "'"
                   << std::endl;
        return _state;
    }
    try {
        Response &response = _strategy->get_response();
        if (!response.is_done())
            response.build_response(_buffer, BUFFER_SIZE);
        int written = 0;
        if (_buffer != "") {
#ifdef DEBUG
            if (response.is_text()) {
                write(1, _buffer.c_str(), (BUFFER_SIZE <= _buffer.length() ? BUFFER_SIZE : _buffer.length()));
                write(1, "\n", 1);
            }
#endif
            written
                = write(_socket, _buffer.c_str(), (BUFFER_SIZE <= _buffer.length() ? BUFFER_SIZE : _buffer.length()));
        }
        if (written < 0) {
            error.log() << "Error while writing response to socket " << _socket << ": " << strerror(errno)
                        << ". Closing connexion" << std::endl;
            _state = s_error; // is_error means to close the connexion
            return _state;    // cause written < 0 won't do well in substr...
        }
        event.log() << "Wrote " << written << " byte(s) in socket " << _socket << std::endl;
        if (static_cast< size_t >(written) < (BUFFER_SIZE <= _buffer.length() ? BUFFER_SIZE : _buffer.length()))
            error.log() << "Partial write in socker " << _socket << ", content may be affected." << std::endl;
        _buffer = _buffer.substr(written, _buffer.length() - written);
        if (response.is_done() && _buffer.length() == 0) {
            debug.log() << "Response is sent." << std::endl;
            info.log() << "Just sent " << response.get_code() << std::endl;
            _state = ready;
            if (isError(response.get_code()) || isRedirection(response.get_code())) {
                info.log() << "Closing connexion because of "
                           << (isError(response.get_code()) ? "error" : "redirection") << " code "
                           << response.get_code() << std::endl;
                _state = s_error;
            }
            return _state;
        }
    } catch (HttpError &e) {
        debug.log() << "Rewinding because of " << e.get_code() << " while sending response." << std::endl;
        _internal_status = e.get_code();
        return _state    = ready; // ready to rewind
    }
    return _state;
}

HttpCode ResponseSendState::get_internal_status() const {
    return _internal_status;
}

void ResponseSendState::save_mem() {
    debug.log() << "(i) ResponseSendState saved mem !" << std::endl;
    shrink_to_fit(_buffer);
    _strategy->save_mem();
}
