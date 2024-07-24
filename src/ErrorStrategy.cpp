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
#include "MemoryHandler.hpp"
#include "ResponseBuildingStrategy.hpp"
#include <cstddef>
#include <new>
#include <ostream>
#include <sstream>
#include <string>

ErrorStrategy::ErrorStrategy(HttpCode code):
    ResponseBuildingStrategy(),
    _code                   (code) {}

ErrorStrategy::~ErrorStrategy() {}

bool ErrorStrategy::build_response() {
    if (_built) {
        warn.log() << "ErrorStrategy : trying to build response, but is already built." << std::endl;
        return _built;
    }
    _response.add_header("Content-Type", "text/html; charset=utf-8");
    _estimated_size = 0; // choose BodyWriterLength
    _response.set_code(_code);
    return _built   = true;
}

bool ErrorStrategy::fill_buffer(std::string &buffer, size_t size) {
    if (_done)
        return _done;
    generateErrorPage(buffer);
    return _done = true;
    (void) size;
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
        mem.deallocate();
        error.log() << "bad_alloc during recovery, running basic error generation in low memory consumption mode."
                    << std::endl;
        buffer = status_string(_code);
    } // TODO: check if memory needs to be reallocated after using this strategy.
      //         check this after response is sent.
}

void ErrorStrategy::save_mem() {
    ;
}
