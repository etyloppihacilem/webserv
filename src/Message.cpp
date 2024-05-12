/* #############################################################################

               """          Message.cpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 17:51
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Message.hpp"
#include "HttpMethods.hpp"
#include "HttpError.hpp"
#include "HttpStatusCodes.hpp"
#include "HttpUtils.hpp"
#include <algorithm>
#include <sstream>
#include <string>

Message::Message():
    _method         (),
    _target         (),
    _header         (),
    _response_header(),
    _body           (),
    _status         (unset),
    _absolute_form  (false) {}

Message::~Message() {}

Message::Message(const Message &other) {
    (void) other;
}

Message &Message::operator=(const Message &other) {
    if (&other == this)
        return (*this);
    return (*this);
}

HttpMethod Message::parse_method(const std::string &method, const size_t &end) {
    if (method.length() < 2 || end > MAX_METHOD)
        throw (HttpError(NotImplemented));
    if (method[0] == 'G' && method.find(method_string(GET)) == 0 && end == 3)
        return (GET);
    if (method[0] == 'P' && method.find(method_string(POST)) == 0 && end == 4)
        return (POST);
    if (method[0] == 'D' && method.find(method_string(DELETE)) == 0 && end == 6)
        return (DELETE);
    throw (HttpError(NotImplemented));
}

static void replace_all(std::string &str, const std::string &to_find, const std::string &to_replace) {
    size_t elem = str.find(to_find, 0);

    while (elem != std::string::npos) {
        str.replace(elem, to_find.length(), to_replace);
        elem = str.find(to_find, 0);
    }
}

void Message::parse_target(const std::string &in, const size_t &pos) {
    size_t  sp_protocol;
    size_t  protocol;

    sp_protocol = in.find_first_of(" \t", pos + 1);
    protocol    = in.find("HTTP", pos);
    if (protocol == std::string::npos || sp_protocol == std::string::npos)
        throw (HttpError(BadRequest));
    if ((protocol - 1) - (pos + 1) > MAX_URI)
        throw (HttpError(URITooLong));
    if (sp_protocol != protocol - 1) {  // there are SP remaining in URI, that is wrong, going for 301 MovedPermanently. IF NOT IN ORIGIN
                                        // FORM, ADD HOST TO LOCATION !!!
        std::string redirect = in.substr(pos + 1, (protocol - 1) - (pos + 1));
        replace_all(redirect,   " ",    "%20");
        replace_all(redirect,   "\t",   "%09");
        throw (HttpError(MovedPermanently, redirect));
    }
    _target = in.substr(pos + 1, (protocol - 1) - (pos + 1));
    size_t host = _target.find("http://");
    if (host == 0) {                // absolute form
        size_t host_end = _target.find("/", 7);
        if (host_end == std::string::npos)
            throw (HttpError(BadRequest));
        _header["Host"] = _target.substr(7, host_end - 7);
        _target         = _target.substr(host_end, _target.length() - host_end);
        _absolute_form  = true;
    } else if (_target[0] != '/')   // not origin form
        throw (HttpError(BadRequest));
}

void Message::parse_header_line(const std::string &in, size_t begin, size_t end) {
    size_t  sep = in.find_first_of(":", begin);
    size_t  ows;

    if (sep > end || sep == std::string::npos)
        throw (HttpError(BadRequest));
    std::string key = in.substr(begin, sep - begin);
    do {
        ows = in.find_first_of(" \t", sep + 1);
        sep++;
    } while (ows == sep);   // sep is at begining of value after this
    do {
        ows = in.find_last_of(" \t", end);
        end--;
    } while (ows == end);   // end is last optional whistspace after this
    if (key == "Host") {
        if (!_absolute_form && _header.find("Host") != _header.end())
            throw (HttpError(BadRequest));
        else if (_absolute_form)
            return;
    }
    if (_header.find(key) != _header.end())
        _header[key] += in.substr(sep, (end + 1) - sep);
    else
        _header[key] = in.substr(sep, (end + 1) - sep);
}

void Message::init_header(const std::string &in) {
    size_t  begin   = in.find("\r\n");
    size_t  end     = in.find("\r\n", begin + 2);

    if (begin == std::string::npos || end == std::string::npos)
        throw (HttpError(BadRequest));
    while (begin + 2 != end) {
        parse_header_line(in, begin + 2, end);
        begin = end;
        end = in.find("\r\n", begin + 2);
        if (begin == std::string::npos || end == std::string::npos)
            throw (HttpError(BadRequest));
    }
}

void Message::parse(const std::string &in) {
    size_t sp = in.find_first_of(" \t");

    try {
        _method = parse_method(in, sp);
    } catch (HttpError &e) {
        _method = none;
        _status = e.get_code();
        return; // double check if anything needs to be done in case of error except returning
    }
    try {
        parse_target(in, sp);
    } catch (HttpError &e) {
        _status = e.get_code();
        return;
    }
    try {} catch (HttpError &e) {
        _status = e.get_code();
        return;
    }
}
