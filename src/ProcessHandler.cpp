/* #####################################################################################################################

               """          ProcessHandler.cpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 14:33
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ProcessHandler.hpp"
#include "ClientRequest.hpp"
#include "EventHandler.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ProcessState.hpp"
#include "ReadState.hpp"
#include "ResponseBuildState.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "ResponseSendState.hpp"
#include "ServerManager.hpp"
#include "StringUtils.hpp"
#include <ctime>
#include <ostream>
#include <string>
#include <unistd.h>

ProcessHandler::ProcessHandler(int socket_fd, int port, std::string client_IP) :
    EventHandler(socket_fd, port),
    _state(0),
    _client_IP(client_IP),
    _rewind_count(0) {
    debug.log() << "ProcessHandler: Client " << _client_IP << " created on socket " << _socket_fd << " from port "
                << _port << "." << std::endl;
}

ProcessHandler::~ProcessHandler() {
    if (_state) {
        debug.log() << "ProcessHandler deletes ProcessState" << std::endl;
        delete _state;
    }
    debug.log() << "ProcessHandler: Client " << _client_IP << " deleted on socket " << _socket_fd << " from port "
                << _port << "." << std::endl;
    close(_socket_fd);
}

time_t ProcessHandler::getTimeout() const {
    // NOTE: refactor with _timeout value being retrieve from the state.
    if (_state == 0)
        return CONNECTION_TIMEOUT;
    if (dynamic_cast< ReadState * >(_state) != 0)
        return REQUEST_TIMEOUT;
    if (dynamic_cast< ResponseBuildState<> * >(_state) != 0)
        return INPUT_TIMEOUT;
    if (dynamic_cast< ResponseSendState * >(_state))
        return OUTPUT_TIMEOUT;
    return CONNECTION_TIMEOUT;
}

void ProcessHandler::handle() {
    event.log() << "ProcessHandler: Client " << _client_IP << " on socket " << _socket_fd << " from port " << _port
                << " received a new event!" << std::endl;
    updateLastsActivity();
    if (!_state)
        _state = new ReadState(_socket_fd, _port, _client_IP);
    if (_state->process() == ready) {
        // passer au state suivant parce qu'il vient de finir.
        if (dynamic_cast< ReadState * >(_state) != 0) {
            transition_to_rbs();
            _state->process();
        }
        if (_state->get_state() == ready && dynamic_cast< ResponseBuildState<> * >(_state) != 0)
            transition_to_rss();
        else if (dynamic_cast< ResponseSendState * >(_state) != 0) {
            if (dynamic_cast< ResponseSendState * >(_state)->get_internal_status() != unset
                && rewind_to_rbs(dynamic_cast< ResponseSendState * >(_state)->get_internal_status()))
                return; // rewind
            ServerManager::getInstance()->listenToClient(_socket_fd, *this);
            clean_state();
            _rewind_count = 0;
            debug.log() << "End of life of state." << std::endl;
        }
    }
    if (_state && _state->get_state() == s_error) {
        ServerManager::getInstance()->deleteClient(_socket_fd, *this);
        return;
    }
}

void ProcessHandler::timeout() {
    warn.log() << "ProcessHandler: Client " << _client_IP << " on socket " << _socket_fd << " on port " << _port
               << " timed out! (" << getTimeout() << "s)" << std::endl;
    ServerManager::getInstance()->deleteClient(_socket_fd, *this);
}

void ProcessHandler::clean_state() {
    if (_state)
        delete _state;
    _state = 0; // needed bc in case of bad_alloc, _state should be 0
}

void ProcessHandler::transition_to_rbs() {
    ReadState *read_state; // to cast
    if ((read_state = dynamic_cast< ReadState * >(_state)) == 0) {
        error.log() << "Transition to ResponseBuildState from a state that is not ReadState. Aborting. Current _state "
                       "is now sending "
                    << InternalServerError << " using recovery ResponseBuildState." << std::endl;
        clean_state();
        _state = new ResponseBuildState<>(_socket_fd, InternalServerError);
        return;
    }
    debug.log() << "ProcessHandler: transitionning from ReadState to ResponseBuildState." << std::endl;
    ClientRequest *request;
    request = read_state->get_client_request();
    clean_state();
    _state = new ResponseBuildState<>(_socket_fd, request, _port); // construction of ResponseBuildState
    debug.log() << "ProcessHandler: transitionning done." << std::endl;
}

void ProcessHandler::transition_to_rss() {
    ResponseBuildState<> *response_build_state; // to cast
    if ((response_build_state = dynamic_cast< ResponseBuildState<> * >(_state)) == 0) {
        error.log() << "Transition to ResponseSendState from a state that is not ResponseBuildState. Aborting. Current "
                       "_state is now sending "
                    << InternalServerError << " using recovery ResponseBuildState." << std::endl;
        clean_state();
        _state = new ResponseBuildState<>(_socket_fd, InternalServerError);
        return;
    }
    debug.log() << "ProcessHandler: transitionning from ResponseBuildState to ResponseSendState." << std::endl;
    ResponseBuildingStrategy *strategy = response_build_state->get_response_strategy();
    clean_state();
    _state = new ResponseSendState(_socket_fd, strategy);
    ServerManager::getInstance()->talkToClient(_socket_fd, *this);
    debug.log() << "ProcessHandler: transitionning done." << std::endl;
}

bool ProcessHandler::rewind_to_rbs(HttpCode code) {
    if (_rewind_count++ > 2)
        return false;
    clean_state();
    if (!isError(code)) {
        warn.log() << code << " is not a proper error code for rewind recovery." << std::endl;
        code = InternalServerError;
    }
    debug.log() << "Rewinding to ResponseBuildState to send " << code << std::endl;
    _state = new ResponseBuildState<>(_socket_fd, code);
    return true;
}

void ProcessHandler::save_mem() {
    debug.log() << "(i) ProcessHandler on " << _client_IP << " " << _port << " saved mem !" << std::endl;
    shrink_to_fit(_client_IP);
    if (_state)
        _state->save_mem();
}
