/* #####################################################################################################################

               """          ResponseSendState.cpp
        -\-    _|__
         |\___/  . \        Created on 24 Jul. 2024 at 14:31
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "ResponseSendState.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ProcessState.hpp"
#include "Response.hpp"
#include <ostream>

// HERE: Coder RESPONSESENDSTATE

ResponseSendState::ResponseSendState(int socket, Response *response) : ProcessState(socket), _response(response) {
    if (!response) {
        error.log() << "ResponseSendState: no response provided, sending " << InternalServerError << std::endl;
        _response = new Response();
    }
}

ResponseSendState::~ResponseSendState() {}

t_state ResponseSendState::process() {
    return _state;
}

void ResponseSendState::save_mem() {
    ; // TODO: coder ça
}
