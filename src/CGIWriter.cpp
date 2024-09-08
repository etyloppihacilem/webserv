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
    _cgi_done(false) {
    if ((_cgi_strategy = dynamic_cast< CGIStrategy * >(&strategy)) == 0) {
        error.log() << "Trying to instantiate CGIWriter with non CGIStrategy strategy, sending " << InternalServerError
                    << std::endl;
        throw HttpError(InternalServerError);
    }
    _init = true;
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
    if (_buffer.length() == 0 && !(_cgi_done = !_cgi_strategy->is_child_alive())) {
        debug.log() << "Waiting for CGI to fill buffer." << std::endl;
        return "";
    }
    if (_length == 0 && _buffer.length() == 0 && _cgi_done) {
        debug.log() << "CGI ended with no body." << std::endl;
        _done = true;
        return "";
    }
    debug.log() << "CGI writer filling buffer of " << size << " byte(s) (currently " << _buffer.length() << " byte(s))"
                << std::endl;
    std::string temp = _buffer.substr(0, (size > _buffer.length() ? _buffer.length() : size));
    _buffer.replace(0, (size > _buffer.length() ? _buffer.length() : size), "");
    std::stringstream st;
    size_t            temp_size = temp.size();
    st << std::hex << temp_size;
    _length += temp_size;
    if (_buffer.length() == 0)
        _done = true;
    if (_cgi_strategy) {
        if (!_cgi_strategy->get_length()) {
            debug.log() << "Sending a message of length 0x" << st.str() << " (" << temp_size << ")" << std::endl;
            temp = st.str() + "\r\n" + temp + (_done && temp_size ? "\r\n0\r\n\r\n" : "\r\n");
        }
    } else {
        debug.log() << "Sending a message of length 0x" << st.str() << " (" << temp_size << ")" << std::endl;
        temp = st.str() + "\r\n" + temp + (_done && temp_size ? "\r\n0\r\n\r\n" : "\r\n");
        error.log() << "CGIWriter is set with no CGIStrategy Strategy. Sending with Chunk by default." << std::endl;
    }
    return temp;
}

bool CGIWriter::read_from_child() {
    debug.log() << "Parent is going to read from child." << std::endl;
    if (!_cgi_done)
        _cgi_done = _strategy->fill_buffer(_buffer, PIPE_BUFFER_SIZE);
    if (_cgi_done)
        debug.log() << "Done reading from child." << std::endl;
    return _cgi_done;
}

bool CGIWriter::init() {
    if (!_init)
        return _init;
    if (sanitize_HTTP_string(_buffer, 0).find("\n\n") == _buffer.npos) {
        if (_buffer[0] == '\n') {
            _buffer.replace(0, 1, "");
            debug.log() << "No headers generated by CGI" << std::endl;
            return _init = false;
        }
        if ((_cgi_done = !_cgi_strategy->is_child_alive())) {
            error.log() << "Init can not be done and MISO has ended, init will never be done, aborting." << std::endl;
            return _init = false;
        }
        return true;
    }
    if (_buffer.find("\n\n") + 2 == _buffer.length() && !(_cgi_done = !_cgi_strategy->is_child_alive()))
        return true; // waiting for possible body before initializing headers
    debug.log() << "Initiating CGI headers" << std::endl;
    size_t found;
    if ((found = _buffer.find("Status:")) != _buffer.npos) {
        size_t end = _buffer.find("\n", found);
        if (end == _buffer.npos) {
            error.log() << "Incorrect Status header formating in CGI response, sending " << InternalServerError
                        << std::endl;
            send_error(InternalServerError);
        }
        debug.log() << "Found Status header, parsing response accordingly" << std::endl;
        found = _buffer.find_first_not_of(" \t", found + 7);
        _response.set_code(_buffer.substr(found, end - (found)));
        _buffer.replace(found, (end + 1) - found, ""); // deleting Status stuff and \n
    }
    debug.log() << "Parsing CGI headers" << std::endl;
    bool length_header = false;
    while (*_buffer.begin() != '\n') {
        size_t end   = _buffer.find("\n");
        found        = _buffer.find(":");
        size_t space = _buffer.find_first_of(" \t");
        if (end == _buffer.npos)
            break;
        if (found == _buffer.npos || found > end || space < found) {
            warn.log() << "CGI header line was discarded because of incorrect header format." << std::endl;
            debug.log() << "Line: " << _buffer.substr(0, end) << std::endl;
            _buffer.replace(0, end + 1, "");
            continue;
        }
        std::string name = _buffer.substr(0, found);
        found            = _buffer.find_first_not_of(" \t", found + 1);
        if (found == _buffer.npos || found >= end) {
            _buffer.replace(0, end + 1, "");
            continue;
        }
        if (name == "Content-Length")
            length_header = true;
        _response.add_header(name, _buffer.substr(found, end - found));
        debug.log() << "CGI added header '" << name << ": " << _buffer.substr(found, end - found) << "'" << std::endl;
        _buffer.replace(0, end + 1, "");
    }
    _buffer.replace(0, 1, ""); // this is a \n
    debug.log() << "CGI reponse init done" << std::endl;
    if ((_cgi_done = !_cgi_strategy->is_child_alive()) && _buffer == "") {
        debug.log() << "No body form CGI." << std::endl;
        _done = true;
    }
    if (!is_done()) {
        debug.log() << "Adding body headers because of CGI" << std::endl;
        if (!length_header) { // here ?
            debug.log() << "CGI did not defined length, sending body in chunked format." << std::endl;
            _response.add_header("Transfer-Encoding", "chunked");
        }
    }
    return _init = false; // init not to be done anymore
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
