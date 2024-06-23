/* #############################################################################

               """          ErrorStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 23 Jun. 2024 at 16:58
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ErrorStrategy.hpp"
#include "HttpStatusCodes.hpp"
#include "ResponseBuildState.hpp"
#include <sstream>
#include <string>

ErrorStrategy::ErrorStrategy(ResponseBuildState &state, HttpCode code, bool recovery):
    ResponseBuildingStrategy(state),
    _code                   (code),
    _recovery               (recovery) {}

ErrorStrategy::~ErrorStrategy() {}

void ErrorStrategy::buildResponse() {
    if (_recovery)
        _response.add_header("Content-Type", "text/html; charset=utf-8");
    else
        ; // TODO mettre le bon mimetype selon le fichier d'erreur
    _response.set_code(_code);
}

bool ErrorStrategy::fill_buffer(std::string &buffer, size_t size) {
    if (_done)
        return _done;
    if (_recovery) {
        generateErrorPage(buffer);
        return _done = true;
    }
    (void) size; // TODO response with size
    return "";
}

/**
  Generates an error page in case of a really bad error.
  (Example : the error page we are looking for does not exist)
  */
void ErrorStrategy::generateErrorPage(std::string &buffer) {
    std::stringstream st;

    st  << "<head><title>" << static_cast<int>(_code) << " error</title></head><body><h1>Error "
        << static_cast<int>(_code) << " " << status_string(_code) << "</h1><div>This error page was "
        << "automatically generated.</div></body>";
    buffer += st.str();
}
