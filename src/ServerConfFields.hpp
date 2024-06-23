#ifndef INCLUDE_SRC_SERVERCONFFIELDS_HPP_
#define INCLUDE_SRC_SERVERCONFFIELDS_HPP_

#include <string>
#include <vector>
#include "Logger.hpp"

#define MIN_FIELD_SIZE 4
#define MAX_FIELD_SIZE 20
#define MAX_PORT 65535
#define MAX_BODY_SIZE INT_MAX
#define MAX_CLIENT_CONNECTION 5
#define CONNECTION_TIMEOUT 180  //second
#define REQUEST_TIMEOUT 60      //second
#define INPUT_TIMEOUT 15        //second
#define OUTPUT_TIMEOUT 180      //second
#define CGI_TIMEOUT 180         //second

#define COUNT_CONF_FIELD 15

typedef std::vector<std::string>            ValueList;
typedef std::pair<std::string, std::string> Field;

enum ConfField{
    // none = -1,
    http                    = 0,
    server                  = 1,
    location                = 2,
    error_page              = 3,
    server_name             = 4,
    listen                  = 5,
    root                    = 6,
    index                   = 7,
    methods                 = 8,
    client_max_body_size    = 9,
    autoindex               = 10,
    rewrite                 = 11,
    upload_path             = 12,
    cgi_path                = 13,
    file_ext                = 14,
};

inline std::string ConfFieldString(const int &code)
{
    switch (code) {
        // case -1:
        //  return "none";
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
            return "methods";
        case 9:
            return "client_max_body_size";
        case 10:
            return "autoindex";
        case 11:
            return "rewrite";
        case 12:
            return "upload_path";
        case 13:
            return "cgi_path";
        case 14:
            return "file_ext";
        default:
            return std::string();
    }
}

class ServerConfError: public std::exception {
    public:
        ServerConfError() throw () {}

        virtual ~ServerConfError() throw () {}
};

class ServerConfWarn: public std::exception {
    public:
        ServerConfWarn() {}
        virtual ~ServerConfWarn() throw () {}
};

#endif  // INCLUDE_SRC_SERVERCONFFIELDS_HPP_
