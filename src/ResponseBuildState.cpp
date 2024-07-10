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
#include "MemoryHandler.hpp" // do not remove yet
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
        throw e; // gestion out of scope ? TODO decider ça : pros gestion de la memoire de tous les objets
        // mem.deallocate();
        // if (_was_error) {
        //     error.log() << "Out of heap, not recoverable, sending " << InternalServerError << std::endl;
        //     init_strategy(InternalServerError);
        //     mem.allocate(); // wtf not now
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
    //  error
    //      error strategy (init_strategy(HttpCode code))
    if (isError(_request->get_status()))
        init_strategy(_request->get_status());
    // else if (isRedirection(_request->get_status()))
    //     ; // init redirectstrategy
    // else if (_request->get_method() == GET)
    //     ;
    //  get;
    //      verifier la location
    //          fichier
    //              GetFile
    //              or CGI thing
    //          dossier
    //              IndexStrategy (ou 403)
    // else if (_request->get_method() == POST)
    //     ;
    //  post;
    //      upload
    // else if (_request->get_method() == DELETE) // or <= for put but change flag
    //  delete;
    //      delete
    // ;
    // trouver le moyen de construire une location
    // and to know what is allowed at this location
}

/**
  Build error strategy for a given http code.
  */
void ResponseBuildState::init_strategy(HttpCode code) {
    if (!isError(code))
        warn.log() << "Generating error for non error code " << code << std::endl;
    try {
        _strategy = new ErrorStrategy(*this, code);
    } catch (std::exception &e) {
        _strategy = new ErrorStrategy(*this, code, true);
    }
}
