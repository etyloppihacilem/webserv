/* #####################################################################################################################

               """          CGIWriter.cpp
        -\-    _|__
         |\___/  . \        Created on 29 Aug. 2024 at 13:28
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "CGIWriter.hpp"
#include "BodyWriter.hpp"
#include "CGIStrategy.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "StringUtils.hpp"
#include "todo.hpp"
#include <cstddef>
#include <ios>
#include <ostream>
#include <sstream>
#include <string>

CGIWriter::CGIWriter(ResponseBuildingStrategy &strategy) :
    BodyWriter(strategy),
    _response(strategy.get_response()),
    _total(0),
    _init(true) {
    if (dynamic_cast< CGIStrategy * >(&strategy) == 0) {
        error.log() << "Trying to instantiate CGIWriter with non CGIStrategy strategy, sending " << InternalServerError
                    << std::endl;
        throw HttpError(InternalServerError);
    }
}

CGIWriter::~CGIWriter() {}

// Generate should be called once before it will send body to init response headers.
std::string CGIWriter::generate(size_t size) {
    if (_done) {
        warn.log() << "CGI body generating was already done." << std::endl;
        return "";
    }
    if (_init) {
        init();
        return "";
    }
    debug.log() << "CGI writer filling buffer of " << size << " byte(s) (currently " << _buffer.length() << " byte(s))"
                << std::endl;
    if (!_cgi_done)
        do {
            _cgi_done = _strategy->fill_buffer(_buffer, PIPE_BUFFER_SIZE);
        } while (_buffer.length() < size && !_cgi_done);
    std::string temp = _buffer.substr(0, (size > _buffer.length() ? _buffer.length() : size));
    _buffer.replace(0, (size > _buffer.length() ? _buffer.length() : size), "");
    std::stringstream st;
    st << std::hex << temp.size();
    _length         += temp.length();
    CGIStrategy *tmp = dynamic_cast< CGIStrategy * >(_strategy);
    if (tmp) {
        if (!tmp->get_length())
            temp = st.str() + "\r\n" + temp + (_done ? "\r\n0\r\n\r\n" : "\r\n");
    } else {
        temp = st.str() + "\r\n" + temp + (_done ? "\r\n0\r\n\r\n" : "\r\n");
        error.log() << "CGIWriter is set with no CGIStrategy Strategy. Sending with Chunk by default." << std::endl;
    }
    if (_buffer.length() == 0)
        _done = true;
    return temp;
}

void CGIWriter::init() {
    if (!_init) {
        warn.log() << "CGI init already done, aborting" << std::endl;
        return;
    }
    debug.log() << "Initiating CGI headers" << std::endl;
    if (!_cgi_done)
        do {
            _cgi_done = _strategy->fill_buffer(_buffer, PIPE_BUFFER_SIZE);
        } while (sanitize_HTTP_string(_buffer, 0).find("\n\n") != _buffer.npos && !_cgi_done);
    size_t found;
    if ((found = _buffer.find("Status:")) != _buffer.npos) {
        size_t end = _buffer.find("\n", found);
        if (end == _buffer.npos) {
            error.log() << "Incorrect Status header formating in CGI response, sending " << InternalServerError
                        << std::endl;
            send_error(InternalServerError);
        }
        debug.log() << "Found Status header, parsing response accordingly" << std::endl;
        found = _buffer.find_first_not_of(" \t", found);
        _response.set_code(_buffer.substr(found, end - found));
        _buffer.replace(found, (end + 1) - found, ""); // deleting Status stuff and \n
    }
    debug.log() << "Parsing CGI headers" << std::endl;
    while (*_buffer.begin() != '\n') {
        size_t end = _buffer.find("\n");
        found      = _buffer.find(":");
        if (end == _buffer.npos)
            break;
        if (found == _buffer.npos || found > end) {
            _buffer.replace(0, end + 1, "");
            continue;
        }
        std::string name = _buffer.substr(0, found);
        found            = _buffer.find_first_not_of(" \t", found + 1);
        if (found == _buffer.npos || found >= end) {
            _buffer.replace(0, end + 1, "");
            continue;
        }
        _response.add_header(name, _buffer.substr(found, end));
        debug.log() << "CGI added header '" << name << ": " << _buffer.substr(found, end) << "'" << std::endl;
        _buffer.replace(0, end + 1, "");
    }
    _buffer.replace(0, 1, ""); // this is a \n
    debug.log() << "CGI reponse init done" << std::endl;
    _init = false; // init done
}

void CGIWriter::send_error(HttpCode code) {
    if (!isError(code))
        warn.log() << "Trying to send CGI error with " << code << ", which is not an error code, sending "
                   << InternalServerError << std::endl;
    _response.set_code(code);
    _response.set_body("CGI error.");
}

size_t CGIWriter::length() const { // tbh i can't remember why this method exists
    return _total;
}

void CGIWriter::save_mem() {
    ; // TODO: code this
}
