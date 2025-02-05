#include "ServerConfSetter.hpp"
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ServerConfFields.hpp"
#include "ServerConfLogging.hpp"
#include "ServerConfValidate.hpp"
#include "StringUtils.hpp"
#include <climits>
#include <cstddef>
#include <limits>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

std::vector< std::string > setFieldServerName(const ValueList &values) {
    std::vector< std::string > ret;

    for (ValueList::const_iterator it = values.begin(); it < values.end(); ++it) {
        if (!isValidIPAddress(*it)) {
            if (!isValidHostname(*it)) {
                warn.log() << "server_name: " << *it << ", is not a valid Hostname." << std::endl;
                continue;
            } else {
                ret.push_back(*it);
                continue;
            }
            warn.log() << "server_name: " << *it << ", is not a a valid IP address." << std::endl;
            continue;
        } else
            ret.push_back(*it);
    }

    if (ret.empty())
        throw ServerConfWarn();
    return ret;
}

int setFieldListen(const ValueList &values) {
    if (values.size() != 1) {
        logInvalidValuesCount(listen_f, values);
        throw ServerConfWarn();
    }

    std::stringstream tmp(values[0]);
    int               ret;
    tmp >> ret;
    if (tmp.fail() || !tmp.eof()) {
        logInvalidIntConvertion(listen_f, values[0]);
        throw ServerConfWarn();
    }

    if (ret < MIN_ROOTLESS_PORT && ret < MAX_PORT) {
        warn.log() << "listen: " << values[0] << ", is not a valid expected inside range [1024-65535]." << std::endl;
        throw ServerConfWarn();
    }

    return ret;
}

std::string setFieldRoot(const ValueList &values) {
    if (values.size() != 1) {
        logInvalidValuesCount(root, values);
        throw ServerConfWarn();
    }

    if (!isValidRelativePath(values[0])) {
        warn.log() << "root: " << values[0] << ", is not a valid root." << std::endl;
        throw ServerConfWarn();
    }

    return add_trailing_slash(values[0]);
}

std::vector< std::string > setFieldIndex(const ValueList &values) {
    std::vector< std::string > ret;

    for (ValueList::const_iterator it = values.begin(); it < values.end(); ++it) {
        if (extract_extension(*it) == "") {
            warn.log() << "index: " << *it << ", is not a valid index page." << std::endl;
            continue;
        }
        ret.push_back(*it);
    }

    if (ret.empty())
        throw ServerConfWarn();
    return ret;
}

bool setFieldAutoindex(const ValueList &values) {
    if (values.size() != 1) {
        logInvalidValuesCount(autoindex, values);
        throw ServerConfWarn();
    }

    if (values[0] == "on")
        return true;
    else if (values[0] == "off")
        return false;
    else {
        warn.log() << "autoindex: " << values[0] << ", is not a valid expected [on/off]." << std::endl;
        throw ServerConfWarn();
    }
}

std::set< HttpMethod > setFieldMethods(const ValueList &values) {
    std::set< HttpMethod > ret;

    for (ValueList::const_iterator it = values.begin(); it < values.end(); ++it)
        if (*it == "GET")
            ret.insert(GET);
        else if (*it == "POST")
            ret.insert(POST);
        else if (*it == "DELETE")
            ret.insert(DELETE);
        else if (*it == "PUT")
            ret.insert(PUT);
        else if (*it == "HEAD")
            ret.insert(HEAD);
        else if (*it == "none")
            ret.insert(last);
        else
            warn.log() << "methods: " << *it << ", is not a valid Http method." << std::endl;

    if (ret.empty()) // use "none" to disable route
        throw ServerConfWarn();
    return ret;
}

std::size_t setFieldMaxBodySize(const ValueList &values) {
    if (values.size() != 1) {
        logInvalidValuesCount(client_max_body_size, values);
        throw ServerConfWarn();
    }

    std::stringstream tmp(values[0]);
    long long         ret;
    tmp >> ret;
    if (tmp.fail() || !tmp.eof()) {
        logInvalidIntConvertion(client_max_body_size, values[0]);
        throw ServerConfWarn();
    }

    if (ret < 1 || static_cast< std::size_t >(ret) > std::numeric_limits< std::size_t >::max()) {
        warn.log() << "max_body_size: " << values[0] << ", is not a valid size number." << std::endl;
        throw ServerConfWarn();
    }

    return static_cast< std::size_t >(ret);
}

HttpCode setFieldErrorPageCode(const ValueList &values) {
    if (values.size() != 2) {
        logInvalidValuesCount(error_page, values);
        throw ServerConfWarn();
    }

    std::stringstream tmp(values[0]);
    int               ret;
    tmp >> ret;
    if (tmp.fail() || !tmp.eof()) {
        logInvalidIntConvertion(listen_f, values[0]);
        throw ServerConfWarn();
    }

    if (status_string(ret) == "" || !isError(ret)) {
        warn.log() << "error_page: " << values[0] << ", is not a valid error code." << std::endl;
        throw ServerConfWarn();
    }
    if (ret > 599) {
        info.log() << "error_page: " << values[0] << ", this error code is not standard because above 599."
                   << std::endl;
    }

    return static_cast< HttpCode >(ret);
}

std::string setFieldErrorPagePath(const ValueList &values) {
    if (values.size() != 2) {
        logInvalidValuesCount(error_page, values);
        throw ServerConfWarn();
    }

    if (extract_extension(values[1]) == "") {
        warn.log() << "error_page: " << values[0] << " " << values[1] << ", is not a valid absolute path to file."
                   << std::endl;
        throw ServerConfWarn();
    }

    return values[1];
}

std::string setFieldLocationPath(const std::string &value) {
    if (!isValidAbsolutePath(value)) {
        warn.log() << "location: " << value << ", is not a valid absolute path to folder." << std::endl;
        throw ServerConfWarn();
    }

    std::string ret(value);
    if (ret != "/" && ret.at(ret.size() - 1) == '/') {
        info.log() << "location: " << value << ", trailing slash has been removed." << std::endl;
        ret.erase(ret.size() - 1, 1);
    }
    return ret;
}

std::string setFieldUploadPath(const ValueList &values) {
    if (values.size() != 1) {
        logInvalidValuesCount(upload_path, values);
        throw ServerConfWarn();
    }
    if (values[0].empty())
        throw ServerConfWarn();

    return add_trailing_slash(values[0]);
}

HttpCode setFieldRewriteCode(const ValueList &values) {
    if (values.size() != 2) {
        logInvalidValuesCount(rewrite, values);
        throw ServerConfWarn();
    }

    std::stringstream tmp(values[0]);
    int               ret;
    tmp >> ret;
    if (tmp.fail() || !tmp.eof()) {
        logInvalidIntConvertion(rewrite, values[0]);
        throw ServerConfWarn();
    }

    if (!isRedirection(ret)) {
        warn.log() << "rewrite: " << values[0] << ", is not a valid redir code." << std::endl;
        throw ServerConfWarn();
    }
    if (ret == 310) {
        warn.log() << "rewrite: " << TooManyRedirects << " cannot redirect, " << TemporaryRedirect
                   << " is used instead." << std::endl;
        ret = 307; // temporary redirect by default if redirection is not a usable http code.
    } else if (ret == 306 || ret > 308) {
        warn.log() << "rewrite: " << ret << " is not recognized by server, " << TemporaryRedirect << " is used instead."
                   << std::endl;
        ret = 307; // temporary redirect by default if redirection is not an existing http code.
    }

    return static_cast< HttpCode >(ret);
}

std::string setFieldCgiPath(const ValueList &values) {
    if (values.size() != 1) {
        logInvalidValuesCount(cgi_path, values);
        throw ServerConfWarn();
    }
    if (values[0].empty())
        throw ServerConfWarn();

    return values[0];
}

std::string setFieldFileExt(const ValueList &values) {
    if (values.size() != 1) {
        logInvalidValuesCount(file_ext, values);
        throw ServerConfWarn();
    }

    if (!isValidExtension(values[0])) {
        warn.log() << "file_ext: " << values[0] << ", is not a valid file extension." << std::endl;
        throw ServerConfWarn();
    }

    return values[0].substr(1);
}
