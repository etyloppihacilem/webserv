/* #############################################################################

               """          CGIStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 26 Jun. 2024 at 11:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "CGIStrategy.hpp"
#include "Body.hpp"
#include "BodyChunk.hpp"
#include "ClientRequest.hpp"
#include "HttpError.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "Response.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <cctype>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <new>
#include <ostream>
#include <sched.h>
#include <signal.h>
#include <sstream>
#include <string.h>
#include <string>
#include <strings.h>
#include <sys/wait.h>
#include <unistd.h>

CGIStrategy::CGIStrategy(const std::string &location, ClientRequest *request, const std::string &path_info) :
    ResponseBuildingStrategy(),
    _location(location),
    _path_info(path_info),
    _request(request),
    _body(0),
    _child(0),
    _state(init),
    _was_dechunked(false) {
    if (!_request) {
        error.log() << "Trying to instantiate CGIStrategy without request, sending " << InternalServerError
                    << std::endl;
        throw HttpError(InternalServerError);
    }
    if (!_request->have_body())
        _body = _request->get_body();
}

CGIStrategy::~CGIStrategy() {
    if (_child)
        kill_child();
}

bool CGIStrategy::build_response() {
    if (_built) {
        warn.log() << "CGIStrategy: build_response called but response is already built." << std::endl;
        return _built;
    }
    if (_state == init) // no poll event
        init_CGI();
    if (_state == loading_body) // listen event
        de_chunk();
    if (_state == launch)
        launch_CGI(_body->length());
    if (_state == running)
        feed_CGI();
    return _built;
}

void CGIStrategy::init_CGI() {
    if (_state != init) {
        warn.log() << "CGIStrategy: init called while not in init _state, abort init" << std::endl;
        return;
    }
    debug.log() << "Running init for CGIStrategy." << std::endl;
    if (_body) {
        if (dynamic_cast< BodyChunk * >(_body) == 0) {
            debug.log() << "CGI request with body type chunk, body needs to be dechunked." << std::endl;
            _state = loading_body;
        } else {
            debug.log() << "CGI request with body type length." << std::endl;
            _state = launch;
        }
    } else {
        debug.log() << "CGI request with no body." << std::endl;
        _state = launch;
    }
    if (pipe(_pipin) < 0) {
        error.log() << "CGIStrategy: Could not open pipe, sending " << InternalServerError << std::endl;
        throw HttpError(InternalServerError);
    }
    if (pipe(_pipout) < 0) {
        close(_pipin[0]);
        close(_pipin[1]);
        error.log() << "CGIStrategy: Could not open pipe, sending " << InternalServerError << std::endl;
        throw HttpError(InternalServerError);
    }
}

void CGIStrategy::de_chunk() {
    _was_dechunked = true;
    if (!_body) {
        warn.log() << "CGIStrategy: dechunking with no body, aborting" << std::endl;
        _state = launch;
        return;
    }
    if (_state != loading_body) {
        warn.log() << "CGIStrategy: dechunking body while _state is not loading_body, aborting." << std::endl;
        return;
    }
    if (!_body->is_done())
        _body->get();
    if (_body->is_done())
        _state = launch;
}

void CGIStrategy::launch_CGI(size_t size) {
    // program separation
    pid_t                                pid = fork();
    std::map< std::string, std::string > env;
    fill_env(env, size);
    if (pid < 0) {
        close(_pipin[0]);
        close(_pipin[1]);
        close(_pipout[0]);
        close(_pipout[1]);
        error.log() << "CGIStrategy: Could not open pipe, sending " << InternalServerError << std::endl;
        throw HttpError(InternalServerError);
    } else if (pid) { // parent
        _child = pid;
        close(_pipin[0]);
        close(_pipout[1]);
        info.log() << "CGIStrategy: Child " << pid << " running." << std::endl;
        _state = running;
    } else { // child
        char **c_env = generate_env(env);
        close(_pipin[1]);
        close(_pipout[0]);
        if (dup2(_pipin[0], 0) < 0)
            error.log() << "Cannot redirect stdin into child." << std::endl;
        if (dup2(_pipout[1], 1) < 0)
            error.log() << "Cannot redirect stdout into child." << std::endl;
        // execve(c_env);
        close(_pipin[0]);
        close(_pipout[1]);
        error.log() << "CGIStrategy: Execve failed to run " << "" << std::endl; // logging is on stderr
        free(c_env);
        _exit(1); // _exit with underscore does not flush STDIO
        // WARN: Check if this is memory safe.
    }
}

void CGIStrategy::feed_CGI() {
    if (_body) {
        std::string &content = _body->get();
        int          ret;
        do {
            ret = write(
                _pipin[1], content.c_str(), (PIPE_BUFFER_SIZE <= content.length() ? PIPE_BUFFER_SIZE : content.length())
            );
            if (ret < 0) {
                close(_pipin[1]);
                close(_pipout[0]);
                error.log() << "CGIStrategy: error while writing in pipe to script, sending " << InternalServerError
                            << std::endl;
                throw HttpError(InternalServerError);
            }
            if (static_cast< size_t >(ret) < content.length())
                content = content.substr(ret, content.length() - ret);
            else
                content = "";
        } while (_was_dechunked && content.length() > 0);
        if (content.length() == 0) {
            close(_pipin[1]);
            debug.log() << "Request is done being fed to CGI" << std::endl;
            _built = true;
        }
    } else {
        debug.log() << "Request has no body, CGI is done being initiated." << std::endl;
        close(_pipin[1]);
        _built = true;
    }
}

void CGIStrategy::fill_env(std::map< std::string, std::string > &env, size_t size) {
    env["CONTENT_TYPE"] = "";
    if (size) {
        std::stringstream st;

        st << size;
        st >> env["CONTENT_LENGTH"];
    }
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["PATH_INFO"]         = _path_info;
    env["PATH_TRANSLATED"]   = _location; // physical path after translation on device
    env["QUERY_STRING"]      = _request->get_query_string();
    env["REMOTE_HOST"]       = "";                 // leave empty
    env["REMOTE_ADDR"]       = _request->get_ip(); // client ip
    env["REQUEST_METHOD"]    = method_string(_request->get_method());
    env["SCRIPT_NAME"]       = _request->get_target();
    env["SERVER_NAME"]       = _request->get_header().at("Host");
    {
        std::stringstream st;
        st << _request->get_port();
        st >> env["SERVER_PORT"];
    }
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_SOFTWAR"]  = SERVER_SOFTWARE;
    for (mapit it = _request->get_header().begin(); it != _request->get_header().end(); it++) {
        std::string name = "HTTP_";
        name            += it->first;
        for (std::string::iterator i = name.begin(); i != name.end(); i++) { // sanitizing name
            if (std::isalnum(*i)) {
                if (std::isalpha(*i))
                    *i = std::toupper(*i);
            } else
                *i = '_';
        }
        env[name] = it->second;
    }
}

char **CGIStrategy::generate_env(const std::map< std::string, std::string > &env) const {
    char **ret = 0;

    if (!(ret = new (std::nothrow) char *[env.size() + 1])) {
        error.log() << "CGIStrategy: env alloc failed. CGI env will not be generated." << std::endl;
        return 0;
    }
    bzero(ret, env.size() + 1);

    size_t i = 0;

    for (std::map< std::string, std::string >::const_iterator it = env.begin(); it != env.end(); it++) {
        std::string tmp = it->first + "=" + it->second;

        if (!(ret[i] = strdup(tmp.c_str()))) { // bad alloc
            error.log() << "CGIStrategy: env alloc partially failed. CGI env will not be generated." << std::endl;
            for (size_t j = 0; j <= env.size(); j++)
                if (ret[j])
                    delete[] ret[j];
            delete[] ret;
            return 0;
        }
    }
    return ret;
}

bool CGIStrategy::fill_buffer(std::string &buffer, size_t size) { // find a way to force chunk
    if (_done)
        return _done;
    char buf[PIPE_BUFFER_SIZE + 1] = { 0 };
    int  rd;
    size_t original = buffer.size();

    while (buffer.size() - original < size && !_done) {
        rd = read(_pipout[0], buf, PIPE_BUFFER_SIZE);
        if (rd < 0) {
            close(_pipout[0]);
            kill_child();
            error.log() << "Error reading in pipe from CGI child. Aborting." << std::endl;
            return _done = true;
        }
        if (std::string(buf).find(EOF) != std::string::npos)
        {
            kill_child();
            close(_pipout[0]);
            _done = true;
        }
        buffer.insert(0, buf);
    }
    return _done;
}

void CGIStrategy::kill_child() {
    if (!_child) {
        warn.log() << "Trying to kill child when there is no child running." << std::endl;
        return;
    }
    kill(_child, SIGKILL);
    waitpid(_child, 0, 0); // just to make sure
    _child = 0;
}

void CGIStrategy::save_mem() {
    _response.save_mem();
}
