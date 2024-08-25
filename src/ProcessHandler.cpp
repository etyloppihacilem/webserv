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
#include "Server.hpp"
#include <ostream>

ProcessHandler::ProcessHandler(int socket, Server &server) : EventHandler(socket, server), _state(0) {}

ProcessHandler::~ProcessHandler() {
    if (_state) {
        debug.log() << "ProcessHandler deletes ProcessState" << std::endl;
        delete _state;
    }
}

void ProcessHandler::handle() {
    if (!_state)
        _state = new ReadState(_socket);
    if (_state->process() == ready) {
        // passer au state suivant parce qu'il vient de finir.
        if (dynamic_cast<ReadState *>(_state) != 0)
            transition_to_rbs();
        if (dynamic_cast<ResponseBuildState<> *>(_state) != 0)
            transition_to_rss();
        if (dynamic_cast<ResponseSendState *>(_state) != 0) {
            delete _state;
            _state = 0;
        }
    }
    if (_state->get_state() == s_error) {
        // TODO: Close connexion here
        return;
    }
}
// TODO: change status of epoll in this function

void ProcessHandler::transition_to_rbs() {
    ReadState *read_state; // to cast
    if ((read_state = dynamic_cast<ReadState *>(_state)) == 0) {
        error.log() << "Transition to ResponseBuildState from a state that is not ReadState. Aborting. Current _state "
                       "is now sending "
                    << InternalServerError << " using recovery ResponseBuildState." << std::endl;
        delete _state;
        _state = new ResponseBuildState<>(_socket, InternalServerError, _server);
        return;
    }
    ClientRequest *request;
    request = read_state->get_client_request();
    delete _state;
    _state = 0;                                                   // needed bc in case of bad_alloc, _state should be 0
    _state = new ResponseBuildState<>(_socket, request, _server); // construction of ResponseBuildState
}

void ProcessHandler::transition_to_rss() {
    ResponseBuildState<> *response_build_state; // to cast
    if ((response_build_state = dynamic_cast<ResponseBuildState<> *>(_state)) == 0) {
        error.log() << "Transition to ResponseSendState from a state that is not ResponseBuildState. Aborting. Current "
                       "_state is now sending "
                    << InternalServerError << " using recovery ResponseBuildState." << std::endl;
        delete _state;
        _state = new ResponseBuildState<>(_socket, InternalServerError, _server);
        return;
    }
    ResponseBuildingStrategy *strategy = response_build_state->get_response_strategy();
    delete _state;
    _state = new ResponseSendState(_socket, strategy);
}
