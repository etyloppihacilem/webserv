/* #############################################################################

               """          ResponseBuildState.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:12
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyWriter.hpp"
#include "ClientRequest.hpp"
#include "ErrorStrategy.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "MemoryHandler.hpp"
#include "ProcessState.hpp"
#include "ResponseBuildState.hpp"
#include "ResponseBuildingStrategy.hpp"
#include <exception>
#include <new>
#include <ostream>

/**
 This class is in charge of choosing the right strategy depending on the request.
 */
ResponseBuildState::ResponseBuildState(int fd, ClientRequest *request):
    ProcessState(fd),
    _request    (request),
    _strategy   (0),
    _was_error  (false) {   // TODO check where to free this once it is allocated
    (void) _request;        // TODO delete this
    (void) _strategy;       // TODO delete this
}

ResponseBuildState::~ResponseBuildState() {}

bool ResponseBuildState::process() {
    (void) _was_error; // TODO delete if the rest is uncommented
    try {
        if (!_strategy)
            init_strategy();
    } catch (HttpError &e) {
        init_strategy(e.get_code());
    } catch (std::bad_alloc &e) {
        throw e; // gestion out of scope ? TODO decider ça
        // mem.deallocate();
        // if (_was_error) {
        //     error.log() << "Out of heap, not recoverable, sending " << InternalServerError << std::endl;
        //     init_strategy(InternalServerError);
        //     mem.allocate();
        //     return true;
        // }
        // warn.log() << "Out of heap. Begining recovery procedures." << std::endl;
        // _request->save_mem();
        // _was_error = true;
        // mem.allocate();
        // process();
    }
    return true;
}

ClientRequest *ResponseBuildState::get_request() {
    return _request;
}

ResponseBuildingStrategy *ResponseBuildState::get_response_strategy() {
    return _strategy;
}

/**
  This function build the right strategy for a given ClientRequest.
  */
void ResponseBuildState::init_strategy() {
    //  +CGI
    //     CGI strategy
    //  error
    //      error strategy (init_strategy(HttpCode code))
    //  redirect
    //      redirect
    //  get;
    //      verifier la location
    //          fichier
    //              GetFile
    //          dossier
    //              IndexStrategy (ou 403)
    //  post;
    //      upload
    //  delete;
    //      delete
    ;
    // trouver le moyen de construire une location
}

void ResponseBuildState::init_strategy(HttpCode code) {
    try {
        _strategy = new ErrorStrategy(*this, code);
    } catch (std::exception) {
        _strategy = new ErrorStrategy(*this, code, true);
    }
}
