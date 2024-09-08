/* #############################################################################

               """          CGIStrategy.cpp
        -\-    _|__
         |\___/  . \        Created on 26 Jun. 2024 at 11:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "CGIStrategy.hpp"
#include "Body.hpp"
#include "CGIHandlerMISO.hpp"
#include "ClientRequest.hpp"
#include "HttpError.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "Response.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "ServerManager.hpp"
#include "todo.hpp"
#include <cctype>
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <new>
#include <ostream>
#include <sched.h>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <strings.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

Logger CGIStrategy::babyphone("./child.log", "CHILD", 8);

CGIStrategy::CGIStrategy(
    const std::string &location,
    ClientRequest     *request,
    const std::string &path_info,
    const std::string &cgi_path,
    size_t             max_size
) :
    ResponseBuildingStrategy(),
    _location(location),
    _path_info(path_info),
    _cgi_path(cgi_path),
    _request(request),
    _body(0),
    _child(0),
    _state(init),
    _is_length(false),
    _max_size(max_size),
    _handlerMISO(0),
    _writer(0),
    _code(OK) {
    if (!_request) {
        error.log() << "Trying to instantiate CGIStrategy without request, sending " << InternalServerError
                    << std::endl;
        throw HttpError(InternalServerError);
    }
    if (_request->have_body()) {
        _body = _request->get_body();
        _request->remove_body();
    }
}

CGIStrategy::~CGIStrategy() {
    if (_child)
        kill_child(true);
    if (_handlerMISO)
        ServerManager::getInstance()->deleteClient(_handlerMISO->getSocketFd(), *_handlerMISO);
    if (_body)
        delete _body;
}

void CGIStrategy::removeMISO() {
    _handlerMISO = 0;
}

bool CGIStrategy::build_response() {
    if (_code != OK) {
        info.log() << "CGIStrategy encountered an error in ChildHandler and will send " << _code << std::endl;
        throw HttpError(_code);
    }
    if (_built || _state == launched) {
        warn.log() << "CGIStrategy: build_response called but response is already built." << std::endl;
        return _built;
    }
    if (_state == init) // no poll event
        init_CGI();
    else if (_state == loading_body) // listen event
        fill_temp_file();
    if (_state == launch)
        launch_CGI(_body ? _body->length() : 0, _body != 0); // or segfault bc there is no body !!!
    return _built;
}

void CGIStrategy::clean_filestream() {
    if (_temp_stream.is_open()) {
        debug.log() << "_temp_stream is still open, removing " << _temp_file << std::endl;
        remove(_temp_file.c_str());
        _temp_stream.close();
    }
}

void CGIStrategy::init_CGI() {
    if (_state != init) {
        warn.log() << "CGIStrategy: init called while not in init _state, abort init" << std::endl;
        return;
    }
    debug.log() << "Running init for CGIStrategy." << std::endl;
    {
        std::stringstream st;
        st << "./temp_" << std::hex << _request << std::dec << "_" << _request->get_socket();
        _temp_file = st.str();
    }
    _temp_stream.open(_temp_file.c_str(), std::ios::trunc | std::ios::out);
    if (!_temp_stream.is_open()) {
        error.log() << "CGI temp file " << _temp_file << " could not be opened, sending " << InternalServerError
                    << std::endl;
        throw HttpError(InternalServerError);
    }
    debug.log() << "Created temp file " << _temp_file << " for CGI." << std::endl;
    if (_body) {
        _state = loading_body;
    } else {
        _temp_stream.close();
        debug.log() << "CGI request with no body." << std::endl;
        _state = launch;
    }
    if (pipe(_miso) < 0) {
        error.log() << "CGIStrategy: Could not open pipe, sending " << InternalServerError << std::endl;
        throw HttpError(InternalServerError);
    }
}

void CGIStrategy::fill_temp_file() {
    if (!_body) {
        warn.log() << "CGIStrategy: dechunking with no body, aborting" << std::endl;
        _state = launch;
        return;
    }
    if (_state != loading_body) {
        warn.log() << "CGIStrategy: dechunking body while _state is not loading_body, aborting." << std::endl;
        return;
    }
    if (!_body->is_done()) {
        _body->read_body();
        _temp_stream << _body->pop();
    }
    if (_body->length() > _max_size) {
        kill_child(true);
        info.log() << "Max body size reached, sending " << ContentTooLarge << std::endl;
        debug.log() << "client_max_body_size = " << _max_size << "(bytes) while body length = " << _body->length()
                    << std::endl;
        throw HttpError(ContentTooLarge);
    }
    if (_body->is_done()) {
        debug.log() << "Body was successfully dechunked, for a total length of " << _body->length() << " bytes."
                    << std::endl;
        _state = launch;
    }
}

void CGIStrategy::launch_CGI(size_t size, bool body) {
    // program separation
    debug.log() << "Preparing to launch CGI " << _cgi_path << " with script " << _location << std::endl;
    std::map< std::string, std::string > env;
    fill_env(env, size);
    pid_t pid = fork();
    if (pid < 0) {
        close(_miso[0]);
        close(_miso[1]);
        error.log() << "CGIStrategy: Could not fork CGI child, sending " << InternalServerError << std::endl;
        throw HttpError(InternalServerError);
    } else if (pid) { // parent
        _child = pid;
        close(_miso[1]);
        info.log() << "CGIStrategy: Child " << pid << " running." << std::endl;
        _writer = new CGIWriter(*this);
        errno   = 0;
        if (fcntl(_miso[0], F_SETFL, O_NONBLOCK) == -1) {
            close(_miso[0]);
            error.log() << "CGIStrategy: unable to add set miso to O_NONBLOCK " + std::string(std::strerror(errno))
                        << std::endl;
            throw HttpError(InternalServerError);
        }
        _handlerMISO = new CGIHandlerMISO(_miso[0], *this, *_writer, _temp_file, _request->get_socket());
        if (ServerManager::getInstance()->addCGIToddler(_handlerMISO) == -1) {
            error.log() << "CGIStrategy: unable to add CGIToddler to epoll list." << std::endl;
            throw HttpError(InternalServerError);
        }
        _temp_stream.close();
        errno = 0;
        _response.set_cgi(this, *_writer);
        _state = launched;
        // _built = true; // CGIStrategy is never done
    } else { // child
#ifndef DEBUG
        close(STDERR_FILENO); // close stderr if not debug
#else
        Logger::unforce();
        debug.disable();
        info.disable();
        warn.disable();
#endif
        close(_miso[0]);
        if (body || 1) {
            int temp_fd = open(_temp_file.c_str(), O_RDONLY);
            if (temp_fd < 0) {
                babyphone.log() << "Cannot open temp file " << _temp_file << " " << strerror(errno) << std::endl;
                close(_miso[1]);
                _exit(1);
            }
            if (dup2(temp_fd, 0) < 0) {
                babyphone.log() << "Cannot redirect stdin into child." << std::endl;
                close(temp_fd);
                close(_miso[1]);
                _exit(1);
            }
            close(temp_fd);
        }
        // close(_miso[1]);
        // _miso[1] = open("./output.log", O_CREAT | O_WRONLY | O_TRUNC, 000666);
        // if (_miso[1] < 0)
        //     babyphone.log() << "Could not open outfile." << std::endl;
        if (dup2(_miso[1], 1) < 0) {
            babyphone.log() << "Cannot redirect stdout into child." << std::endl;
            close(_miso[1]);
            _exit(1);
        }
        close(_miso[1]);
        char **args = new (std::nothrow) char *[3];
        args[1]     = strdup(_location.c_str());
        if (!args || !args[1]) {
            if (args)
                free(args);
            babyphone.log() << "Cannot creat arg string. Aborting." << std::endl;
            _exit(1);
        }
        args[2]      = 0;
        char **c_env = generate_env(env);
        if (!c_env) { // nothing gets past this line
            free(args[0]);
            free(args);
            babyphone.log() << "Cannot creat arg string. Aborting." << std::endl;
            _exit(1);
        }
        char *cmd = strdup(_cgi_path.c_str());
        args[0]   = cmd;
        if (!cmd) {
            free(c_env);
            free(args[0]);
            free(args);
            babyphone.log() << "Cannot create arg string. Aborting." << std::endl;
            _exit(1);
        }
        babyphone.log() << "Running execve(" << cmd << ", " << args[1] << ", env)" << std::endl;
        ServerManager::getInstance()->deleteInstance(); // TS-14
        execve(cmd, args, c_env);
        babyphone.log() << "CGIStrategy: Execve failed to run " << strerror(errno) << std::endl; // logging is on stderr
        free(c_env);
        free(args[0]);
        free(args);
        _exit(1); // _exit with underscore does not flush STDIO"
    }
}

// CONTENT_TYPE="" GATEWAY_INTERFACE=CGI/1.1 HTTP_ACCEPT_ENCODING=gzip HTTP_HOST=127.0.0.1
// HTTP_USER_AGENT=Go-http-client/1.1 PATH_INFO="" QUERY_STRING="" REMOTE_ADDR=2.0.164.200 REMOTE_HOST=""
// REQUEST_METHOD=GET SCRIPT_NAME=/directory/youpi.bla SERVER_NAME=127.0.0.1 SERVER_PORT=8080 SERVER_PROTOCOL=HTTP/1.1
// SERVER_SOFTWAR=jeon/0.1 PATH_TRANSLATED=YoupiBananerectory/youpi.bla

void CGIStrategy::fill_env(std::map< std::string, std::string > &env, size_t size) {
    env["CONTENT_TYPE"] = "";
    if (size) {
        std::stringstream st;

        st << size;
        st >> env["CONTENT_LENGTH"];
    }
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    // env["PATH_INFO"]         = _path_info; // RFC 3875 friendly
    env["PATH_INFO"]         = _request->get_target();
    env["REQUEST_URI"]       = _request->get_target();
    env["PATH_TRANSLATED"]   = _location; // physical path after translation on device
    env["QUERY_STRING"]      = _request->get_query_string();
    env["REMOTE_HOST"]       = "";                 // leave empty
    env["REMOTE_ADDR"]       = _request->get_ip(); // client ip
    env["REQUEST_METHOD"]    = method_string(_request->get_method());
    env["SCRIPT_NAME"]       = _request->get_target();
    env["SERVER_NAME"]       = _request->get_header().at("Host");
    debug.log() << "PATH_INFO=" << env["PATH_INFO"] << std::endl;
    debug.log() << "REQUEST_URI=" << env["REQUEST_URI"] << std::endl;
    {
        std::stringstream st;
        st << _request->get_port();
        st >> env["SERVER_PORT"];
    }
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
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
    char       **ret          = 0;
    const char **sys_env      = ServerManager::getEnv();
    size_t       sys_env_size = 0;

    if (!sys_env)
        babyphone.log() << "sys_env is null, and will no be used." << std::endl;
    else {
        for (size_t i = 0; sys_env[i] != 0; i++) {
            std::string env_str = sys_env[i];
            if (env_str.find("=") != env_str.npos) {
                env_str.resize(env_str.find("="));
                if (env.find(env_str) != env.end())
                    continue; // do not increment size because wont be saved
            }
            sys_env_size++;
        }
        babyphone.log() << "sys_env.length is " << sys_env_size << std::endl;
    }
    if (!(ret = new (std::nothrow) char *[env.size() + sys_env_size + 1])) {
        babyphone.log() << "CGIStrategy: env alloc failed. CGI env will not be generated." << std::endl;
        return 0;
    }
    bzero(ret, sizeof(char *) * (env.size() + sys_env_size + 1));

    size_t i = 0;

    for (std::map< std::string, std::string >::const_iterator it = env.begin(); it != env.end(); it++) {
        std::string tmp = it->first + "=" + it->second;
        babyphone.log() << "Adding to CGI env " << tmp << std::endl;
        if (!(ret[i++] = strdup(tmp.c_str()))) { // bad alloc is fine
            babyphone.log() << "CGIStrategy: env alloc partially failed. CGI env will not be generated." << std::endl;
            for (size_t j = 0; j <= env.size() + sys_env_size; j++)
                if (ret[j])
                    delete[] ret[j];
            delete[] ret;
            return 0;
        }
    }
    for (size_t it = 0; sys_env[it] != 0; it++) { // adding base env
        std::string env_str = sys_env[it];
        if (env_str.find("=") != env_str.npos) {
            env_str.resize(env_str.find("="));
            if (env.find(env_str) != env.end()) {
                babyphone.log() << "Skipping " << sys_env[it] << " because " << env_str << " is CGI reserved."
                                << std::endl;
                continue; // do not increment size because wont be saved
            }
        }
        babyphone.log() << "Adding base to CGI env " << sys_env[it] << std::endl;
        if (!(ret[i++] = strdup(sys_env[it]))) { // bad alloc is fine
            babyphone.log() << "CGIStrategy: env alloc partially failed. CGI env will not be generated." << std::endl;
            for (size_t j = 0; j <= env.size() + sys_env_size; j++)
                if (ret[j])
                    delete[] ret[j];
            delete[] ret;
            return 0;
        }
    }
    return ret;
}

bool CGIStrategy::fill_buffer(std::string &buffer, size_t size) { // find a way to force chunk
    if (_done)                                                    // not done
        return _done;
    int rd;
    (void) size;

    if (!_done) {
        char buf[PIPE_BUFFER_SIZE + 1] = { 0 };
        rd                             = read(_miso[0], buf, PIPE_BUFFER_SIZE);
        if (rd < 0) {
            kill_child(true); // true bc we want to execute child
            // kill_child(false); // debug only
            error.log() << "Error reading in pipe from CGI child. Aborting." << std::endl;
            return _done = true;
        }
        if (rd == 0) {
            if (_child) {
                debug.log() << "Done reading from CGI pipe, waiting for child." << std::endl;
                kill_child(false);
                _done = true;
            }
        }
        debug.log() << "Read " << rd << " byte(s) from CGI pipe." << std::endl;
#ifdef DEBUG
        if (rd > 0) {
            write(1, buf, rd);
            write(1, "\n", 1);
        }
#endif
        buffer += std::string(buf);
    }
    return _done;
}

bool CGIStrategy::is_child_alive() {
    if (!_child) {
        debug.log() << "Child already died." << std::endl;
        return false;
    }
    int waitinfo;
    waitpid(_child, &waitinfo, WNOHANG);
    if (WIFEXITED(waitinfo) == 0) {
        debug.log() << "Child is not dead yet." << std::endl;
        return true;
    }
    debug.log() << "Child " << _child << " have died with exit code " << WEXITSTATUS(waitinfo) << std::endl;
    if (WIFSIGNALED(waitinfo))
        debug.log() << "Child was terminated by signal " << WTERMSIG(waitinfo) << " " << strsignal(WTERMSIG(waitinfo))
                    << std::endl;
    _child = 0;
    return false;
}

void CGIStrategy::kill_child(bool k) {
    if (!_child) {
        warn.log() << "Trying to kill child when there is no child running." << std::endl;
        return;
    }
    if (k)
        kill(_child, SIGKILL);
    int status;
    int exit_code;
    waitpid(_child, &status, 0); // just to make sure
    if (WIFEXITED(status)) {
        exit_code = WEXITSTATUS(status);
        debug.log() << "child exited with exit code " << exit_code << "." << std::endl;
        if (WIFSIGNALED(status))
            debug.log() << "Child was terminated by signal " << WTERMSIG(status) << " " << strsignal(WTERMSIG(status))
                        << std::endl;
    } else {
        debug.log() << "child not exited live in limbs of memory." << std::endl;
    }
    clean_filestream();
    _child = 0;
}

// void CGIStrategy::set_length(bool len) {
//     _is_length = len;
// }

void CGIStrategy::is_done_building() {
    _built = true;
}

bool CGIStrategy::get_length() const {
    return _is_length;
}

pid_t CGIStrategy::get_child_pid() const {
    return _child;
}

bool CGIStrategy::MISO_alive() const {
    return _handlerMISO != 0;
}

void CGIStrategy::save_mem() {
    _response.save_mem();
}
