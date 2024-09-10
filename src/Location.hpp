/* #############################################################################

               """          Location.hpp
        -\-    _|__
         |\___/  . \        Created on 10 Jul. 2024 at 10:19
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_LOCATION_HPP_
#define INCLUDE_SRC_LOCATION_HPP_

/**
  This class is meant to interpret a target and convert it to a location, giving what is allowed to do or not.
  */

#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include "Route.hpp"
#include "Server.hpp"
#include <cstddef>
#include <set>
#include <string>
#ifdef TESTING
# include "gtest/gtest.h"
#endif

template < class ServerClass = Server, class RouteClass = Route > // template is there for testing purposes
class Location {
    public:
        Location(std::string target, const ServerClass &server);
        ~Location();

        bool               has_autoindex() const;
        bool               is_file() const;
        bool               is_cgi() const;
        bool               is_redirect() const;
        bool               is_diff() const;
        HttpCode           get_status_code() const;
        HttpCode           get_default_error() const;
        const std::string &get_route() const;
        const std::string &get_path() const;
        const std::string &get_upload_path() const;
        const std::string &get_route_path() const;
        const std::string &get_path_info() const;
        const std::string &get_cgi_path() const;
        bool               has_method(HttpMethod method) const;
        std::size_t        get_max_body_size() const;

    private:
        Location(); // for testing purposes only
        void build_path(const std::string &target, const RouteClass &route);
        void build_path(const std::string &target, const RouteClass &route, const std::string &redirect);
        void setup_cgi(const RouteClass &route);
        bool find_index(const RouteClass &route, struct stat &buf);
        bool stat_file(const std::string &target, struct stat &buf);
        void set_redir(const std::string &target, const RouteClass &route);
        bool check_cgi_glob(const std::string &target, const ServerClass &server);
        bool init_cgi_glob(const std::string &target, const RouteClass &route);

        // bool        _is_put;
        bool                   _autoindex;
        bool                   _is_file;
        bool                   _is_cgi;
        bool                   _is_redirect;
        bool                   _is_diff; ///< If uploadpath and rootdir differs
        std::set< HttpMethod > _methods;
        std::size_t            _max_body_size;
        HttpCode               _status_code;
        HttpCode               _default_error;
        std::string            _route;       ///< Route location in config
        std::string            _cgi_path;    ///< Where is the executable
        std::string            _path;        ///< path to file (or script)
        std::string            _upload_path; ///< path to upload files
        std::string            _route_path;  ///< path to route root dir
        std::string            _path_info;   ///< Everyting that is after the route path.
                                             /**<
                                               ```
                                               /test/cgi.bin/monscript.cgi/path_info
                                                                          ^^^^^^^^^^
                                               ```
                                               */
#ifdef TESTING
        FRIEND_TEST(LocationTestSuite, BuildPathTest);
        FRIEND_TEST(LocationTestSuite, BuildCursorPathTest);
        FRIEND_TEST(LocationTestSuite, BuildPathRedirectTest);
#endif
};

#endif // INCLUDE_SRC_LOCATION_HPP_
