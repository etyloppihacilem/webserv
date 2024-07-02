/* #############################################################################

               """          ErrorStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 23 Jun. 2024 at 16:58
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ErrorStrategy.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ResponseBuildState.hpp"
#include "ResponseBuildingStrategy.hpp"
#include <cstddef>
#include <new>
#include <ostream>
#include <sstream>
#include <string>

ErrorStrategy::ErrorStrategy(ResponseBuildState &state, HttpCode code, bool recovery):
    ResponseBuildingStrategy(state),
    _code                   (code),
    _recovery               (recovery) {}

ErrorStrategy::~ErrorStrategy() {}

// TODO finir ErrorStrategy !!!

bool ErrorStrategy::build_response() {
    if (_recovery) {
        _response.add_header("Content-Type", "text/html; charset=utf-8");
        _estimated_size = 0; // choose BodyWriterLength
    } else
        (void) _response; // TODO mettre le bon mimetype selon le fichier d'erreur
    _response.set_code(_code);
    return _built = true;
}

bool ErrorStrategy::fill_buffer(std::string &buffer, size_t size) {
    if (_done)
        return _done;
    if (_recovery) {
        generateErrorPage(buffer);
        return _done = true;
    }
    (void) size; // TODO response with size in case not a recovery
    return "";
}

/**
  Generates an error page in case of a really bad error.
  (Example : the error page we are looking for does not exist)
  */
void ErrorStrategy::generateErrorPage(std::string &buffer) {
    try {
        std::stringstream st;

        st  << "<head><title>" << static_cast<int>(_code) << " Error</title></head><body><h1>Error: "
            << static_cast<int>(_code) << " " << status_string(_code) << "</h1><div>This error page was "
            << "automatically generated.</div></body>";
        buffer += st.str();
    } catch (std::bad_alloc &e) { // in case of heap going missing.
        error.log() << "bad_alloc during recovery, running basic error generation." << std::endl;
        buffer += status_string(_code);
    }
}

void ErrorStrategy::save_mem() {
    ;
}
