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

HttpMethod Message::parse_method(const std::string &method) {
    if (method.length() < 2)
        throw HttpError(NotImplemented);
    if (method[0] == 'G' && method == method_string(GET))
        return (GET);
    if (method[0] == 'P' && method == method_string(POST))
        return (POST);
    if (method[0] == 'D' && method == method_string(DELETE))
        return (DELETE);
    throw HttpError(NotImplemented);
}

void Message::parse(const std::string &in) {
    size_t sp = in.find_first_of(" \t");

    try {
        parse_method(in.substr(0, sp));
    } catch (HttpError &e){
        _status = e.get_code();
    }
}
