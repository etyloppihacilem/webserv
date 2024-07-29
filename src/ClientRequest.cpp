/* #############################################################################

               """          ClientRequest.cpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 17:51
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Body.hpp"
#include "BodyChunk.hpp"
#include "BodyLength.hpp"
#include "ClientRequest.hpp"
#include "HttpError.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "HttpUtils.hpp"
#include "Logger.hpp"
#include "StringUtils.hpp"
#include <cstddef>
#include <ios>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

ClientRequest::ClientRequest(int fd):
    _fd             (fd),
    _method         (none),
    _target         (),
    _header         (),
    _body_exists    (),
    _body           (0),
    _status         (OK),
    _absolute_form  (false),
    _port           (80) {}

ClientRequest::ClientRequest(int fd, HttpCode code, int port):
    _fd             (fd),
    _method         (none),
    _target         (),
    _header         (),
    _body_exists    (),
    _body           (0),
    _status         (code),
    _absolute_form  (false),
    _port           (port) {
    if (!isError(_status))
        _status = BadRequest;
}

ClientRequest::~ClientRequest() {
    if (_body)
        delete _body;
}

HttpMethod ClientRequest::parse_method(const std::string &method, const size_t &end) {
    if (method.length() < 2 || end > MAX_METHOD)
        throw HttpError(NotImplemented);
    if (method[0] == 'G' && method.find(method_string(GET)) == 0 && end == 3)
        return GET;
    if (method[0] == 'P' && method.find(method_string(POST)) == 0 && end == 4)
        return POST;
    if (method[0] == 'D' && method.find(method_string(DELETE)) == 0 && end == 6)
        return DELETE;
    throw HttpError(NotImplemented);
}

static void replace_all(std::string &str, const std::string &to_find, const std::string &to_replace) {
    size_t elem = str.find(to_find, 0);

    while (elem != std::string::npos) {
        str.replace(elem, to_find.length(), to_replace);
        elem = str.find(to_find, 0);
    }
}

/**
  Parse target. If is origin form, parses it normally, and if is absolute form, parses it in a way to make it normal
  and adding Host header.
  */
void ClientRequest::parse_target(const std::string &in, const size_t &pos) {
    {
        size_t  sp_protocol;
        size_t  protocol;

        sp_protocol = in.find_first_of(" \t", pos + 1);
        protocol    = in.find("HTTP", pos);
        if (protocol == std::string::npos || sp_protocol == std::string::npos || protocol + 8 != in.find("\r\n", pos))
            throw HttpError(BadRequest);
        if (in.substr(protocol, 7) != "HTTP/1." || (in[protocol + 7] != '0' && in[protocol + 7] != '1'))
            throw HttpError(HTTPVersionNotSupported);
        if ((protocol - 1) - (pos + 1) > MAX_URI)
            throw HttpError(URITooLong);
        if (sp_protocol != protocol - 1) {  // there are SP remaining in URI, that is wrong, going for
                                            // 301 MovedPermanently.
            std::string redirect = in.substr(pos + 1, (protocol - 1) - (pos + 1));

            replace_all(redirect,   " ",    "%20");
            replace_all(redirect,   "\t",   "%09");
            throw HttpError(MovedPermanently, redirect); // message is the redirect location
        }
        _target = in.substr(pos + 1, (protocol - 1) - (pos + 1));   // if origin form, nothing to do because target is
                                                                    // already right.
    }
    size_t host = _target.find("http://");

    if (host == 0) { // absolute form
        size_t host_end = _target.find("/", 7);

        if (host_end == std::string::npos || host_end == 7)
            throw HttpError(BadRequest);
        _header["Host"] = _target.substr(7, host_end - 7);
        _target         = _target.substr(host_end, _target.length() - host_end);
        _absolute_form  = true;
    } else if (_target[0] != '/') // not origin form
        throw HttpError(BadRequest);
}

/**
  Parses a header line.

  In case header already exists, it adds it to the existing value.

  In case header is Host, if request is in absolute form it does nothing, and if it is in origin form it checks if
  Host is not yet defined.
  */
void ClientRequest::parse_header_line(const std::string &in, size_t begin, size_t end) {
    size_t  sep = in.find_first_of(":", begin);
    size_t  ows = in.find_first_of(" \t", begin);

    if (ows < sep || sep > end || sep <= begin || sep == std::string::npos)
        throw HttpError(BadRequest);

    std::string key = in.substr(begin, sep - begin);

    do {
        ows = in.find_first_of(" \t", sep + 1);
        sep++;
    } while (ows == sep);   // sep is at begining of value after this
    do {
        ows = in.find_last_of(" \t", end - 1);
        end--;
    } while (ows == end);   // end is last optional whistspace after this
    if (key == "Host") {
        if (!_absolute_form && _header.find("Host") != _header.end())
            throw HttpError(BadRequest);
        else if (_absolute_form)
            return;
    }
    if (end + 1 <= sep)
        throw HttpError(BadRequest);
    else if (_header.find(key) != _header.end())
        _header[key] += ", " + in.substr(sep, (end + 1) - sep);
    else
        _header[key] = in.substr(sep, (end + 1) - sep);
}

void ClientRequest::init_header(const std::string &in) {
    size_t  begin   = in.find("\r\n");
    size_t  end     = in.find("\r\n", begin + 2);

    if (begin == std::string::npos || end == std::string::npos)
        throw HttpError(BadRequest);
    while (begin + 2 != end) {
        parse_header_line(in, begin + 2, end);
        begin   = end;
        end     = in.find("\r\n", begin + 2);
        if (begin == std::string::npos || end == std::string::npos)
            throw HttpError(BadRequest);
    }
    if (_header.find("Host") == _header.end())
        throw HttpError(BadRequest);
}

/// Parsing header of request.

/**
  Will first parse the method, then the target and parameters, and the headers.

  It returns true if it is ready to continue or false if an error occurred.
  The `_status` code is set accordingly and should be use to determine which error page to reply with. When false is
  returned, this does not guarantee that the whole object is set, and therefor it should not be used normally (like
  continuing the response generation).

  In the particular case of a 3xx code (redirect), the only item in `_header` should be 'Location', containing a string to
  redirect to. This should be the *only* header set.
  */
bool ClientRequest::parse_header(const std::string &in) {
    {
        size_t sp = in.find_first_of(" \t");

        try {
            _method = parse_method(in, sp);
        } catch (HttpError &e) {
            _method = none;
            _status = e.get_code();
            return false;
        }
        try {
            parse_target(in, sp);
            parse_parameters();
            decode_target();
        } catch (HttpError &e) {
            _status = e.get_code();
            if (_status == MovedPermanently)
                _header["Location"] = e.get_message();  // Location is the only header to redirect
                                                        // it is the only header at all because init_header isnt called.
            return false;
        }
    }
    try {
        init_header(in);
        parse_port();
    } catch (HttpError &e) {
        _status = e.get_code();
        return false;
    }
    return true;
}

/**
  This function is used to initiate body (if any).

  It returns true if a body is found, false if not and sets _body_exists accordingly.
  _body_exists should be checked before any action is performed on a supposed _body.
  TODO:check if there is no better way to protect this (in getter for example).
  */
bool ClientRequest::init_body(std::string &buffer) {
    if (_header.find("Transfer-Encoding") != _header.end()) {
        _body_exists    = true;
        _body           = new BodyChunk(_fd, buffer);
    } else if (_header.find("Content-Length") != _header.end()) {
        _body_exists    = true;
        _body           = new BodyLength(_fd, buffer, _header["Content-Length"]);
    } else { // no body
        _body_exists = false;
    }
    return _body_exists;
}

/**
    Replace every % encoded character by its char.
    Format is :
    ```
    %HEX
    ```
    `HEX` being a 2 digit hex value.
  */
void ClientRequest::decode_target() {
    size_t  percent;
    int     c;

    while ((percent = _target.find('%')) != _target.npos) {
        std::stringstream st;

        st << std::hex << _target.substr(percent + 1, 2);
        st >> c;
        _target.replace(percent, 3, 1, c);
    }
}

/**
  This function will init _parameters depending on _target value. It will update and clean _target too.

  Syntax :
  ```
  target/stuff?parameter=value&parameter2=value
  ```
  If there is no value, the value of parameter will be an empty string.
  */

/*
   All of this is not used because no one needs a parsed query string.
void ClientRequest::parse_parameters() {
    size_t  first;
    size_t  begin;
    size_t  end;
    size_t  equal;

    first   = _target.find('?');
    begin   = first;
    while (begin != _target.npos) {
        end     = _target.find('&', begin + 1); // end CAN be _target.npos update it cant
        equal   = _target.find('=', begin);
        if (end == _target.npos && begin + 1 == _target.length())
            ;
        else if (equal >= end)                  // if there is no value
            _parameters[_target.substr(begin + 1, end - (begin + 1))] = "";
        else
            _parameters[_target.substr(begin + 1, equal - (begin + 1))] = _target.substr(equal + 1, end - (equal + 1));
        begin = end;
    }
    if (first != _target.npos)
        _target = _target.substr(0, first);
}

std::string ClientRequest::get_query_string() const {
    std::map<std::string, std::string>::const_iterator it = _parameters.begin();
    std::string ret;

    while (it != _parameters.end()) {
        ret += it->first + "=" + it->second;
        if (++it != _parameters.end())
            ret += "&";
    }
    return ret;
} */
void ClientRequest::parse_parameters() {
    size_t mark;

    mark = _target.find('?');
    if (mark == _target.npos)
        return;
    _query_string = _target.substr(mark + 1, _target.length() - (mark + 1));
    _target.resize(mark);
}

std::string ClientRequest::get_query_string() const {
    return _query_string;
}

/**
  Parse port off Host.
  */
void ClientRequest::parse_port() {
    if (_header.find("Host") == _header.end())
        throw HttpError(BadRequest); // missing host header

    std::string &host   = _header["Host"];
    size_t      sep     = host.find(':');

    if (sep != host.npos) {
        std::stringstream st;

        st << host.substr(sep + 1, host.length() - (sep + 1));
        host.resize(sep); // removing end of host
        if (!(st >> _port))
            throw HttpError(BadRequest);
    }
}

std::string ClientRequest::get_target() const {
    if (_status != OK)
        warn.log() << "Getting target of a request with non " << OK << " status code: " << _status << std::endl;
    return _target;
}

HttpMethod ClientRequest::get_method() {
    if (_status != OK)
        warn.log() << "Getting method of a request with non " << OK << " status code: " << _status << std::endl;
    return _method;
}

Body *ClientRequest::get_body() {
    if (!_body_exists)
        warn.log() << "Getting a request body that does not exists." << std::endl;
    if (_status != OK)
        warn.log() << "Getting body of a request with non " << OK << " status code: " << _status << std::endl;
    return _body;
}

std::map<std::string, std::string> &ClientRequest::get_header() {
    return _header;
}

/* std::map<std::string, std::string> &ClientRequest::get_parameters() {
    return _parameters;
} */
bool ClientRequest::have_body() const {
    return _body_exists;
}

HttpCode ClientRequest::get_status() const {
    return _status;
}

int ClientRequest::get_fd() const {
    return _fd;
}

int ClientRequest::get_port() const {
    if (_status != OK)
        warn.log() << "Getting port of a request with non " << OK << " status code: " << _status << std::endl;
    return _port;
}

void ClientRequest::save_mem() {
    shrink_to_fit(_target);
    _body->save_mem();
}
