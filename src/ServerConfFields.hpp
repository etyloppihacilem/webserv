#ifndef INCLUDE_SRC_SERVERCONFFIELDS_HPP_
#define INCLUDE_SRC_SERVERCONFFIELDS_HPP_

#include "StringTokenizer.hpp"
#include <exception>
#include <string>
#include <utility>
#include <vector>

#define MIN_FIELD_SIZE 4
#define MAX_FIELD_SIZE 20
#define MIN_ROOTLESS_PORT 1024
#define MAX_PORT 65535
#define MAX_BODY_SIZE 2147483648

typedef std::pair< std::string, StringTokenizer > Field;
typedef std::vector< std::string >                ValueList;

#define COUNT_CONF_FIELD 15

enum ConfField {
    wrongField           = -1,
    http                 = 0,
    server               = 1,
    location             = 2,
    error_page           = 3,
    server_name          = 4,
    listen_f             = 5,
    root                 = 6,
    index_f              = 7, // because it does not work on a mac
    autoindex            = 8,
    methods              = 9,
    client_max_body_size = 10,
    upload_path          = 11,
    rewrite              = 12,
    cgi_path             = 13,
    file_ext             = 14,
};

inline std::string ConfFieldString(const int code) {
    switch (code) {
        case 0:
            return "http";
        case 1:
            return "server";
        case 2:
            return "location";
        case 3:
            return "error_page";
        case 4:
            return "server_name";
        case 5:
            return "listen";
        case 6:
            return "root";
        case 7:
            return "index";
        case 8:
            return "autoindex";
        case 9:
            return "methods";
        case 10:
            return "client_max_body_size";
        case 11:
            return "upload_path";
        case 12:
            return "rewrite";
        case 13:
            return "cgi_path";
        case 14:
            return "file_ext";
        default:
            return std::string();
    }
}

inline ConfField isServerConfField(const ConfField code) {
    switch (code) {
        case 2:
            return location;
        case 3:
            return error_page;
        case 4:
            return server_name;
        case 5:
            return listen_f;
        case 6:
            return root;
        case 7:
            return index_f;
        case 8:
            return autoindex;
        case 9:
            return methods;
        case 10:
            return client_max_body_size;
        default:
            return wrongField;
    }
}

inline ConfField isRouteConfField(const ConfField code) {
    switch (code) {
        case 6:
            return root;
        case 7:
            return index_f;
        case 8:
            return methods;
        case 9:
            return autoindex;
        case 10:
            return client_max_body_size;
        case 11:
            return rewrite;
        case 12:
            return upload_path;
        case 13:
            return cgi_path;
        case 14:
            return file_ext;
        default:
            return wrongField;
    }
}

class ServerConfError : public std::exception {
    public:
        ServerConfError() throw() {}

        virtual ~ServerConfError() throw() {}
};

class ServerConfWarn : public std::exception {
    public:
        ServerConfWarn() {}

        virtual ~ServerConfWarn() throw() {}
};

#endif // INCLUDE_SRC_SERVERCONFFIELDS_HPP_
