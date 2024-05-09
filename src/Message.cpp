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
    _method(),
    _target(),
    _header(),
    _body  (),
    _status(unset) {}

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
        throw HttpError(NotImplemented);
    if (method[0] == 'G' && method.find(method_string(GET)) == 0 && end == 3)
        return (GET);
    if (method[0] == 'P' && method.find(method_string(POST)) == 0 && end == 4)
        return (POST);
    if (method[0] == 'D' && method.find(method_string(DELETE)) == 0 && end == 6)
        return (DELETE);
    throw HttpError(NotImplemented);
}

void Message::parse(const std::string &in) {
    size_t sp = in.find_first_of(" \t");
    size_t protocol;

    try {
        _method = parse_method(in, sp);
    } catch (HttpError &e) {
        _method = none;
        _status = e.get_code();
        return; // double check if anything needs to be done in case of error except returning
    }
    sp       = in.find_first_of(" \t", sp);
    protocol = in.find("HTTP", sp);
    if (sp != protocol - 1) {
        ;
    }
}
