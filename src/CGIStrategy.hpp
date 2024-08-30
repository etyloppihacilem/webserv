/* #############################################################################

               """          CGIStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 26 Jun. 2024 at 11:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_CGISTRATEGY_HPP_
#define INCLUDE_SRC_CGISTRATEGY_HPP_

#include "Body.hpp"
#include "ClientRequest.hpp"
#include "Logger.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <cstddef>
#include <map>
#include <sched.h>
#include <string>

typedef enum s_cgistate {
    init = 0,     // first init, run only one time
    loading_body, // if body needs to be dechunked, run as long as body takes to be read entierely
    launch,       // launch CGI
    running,      // running CGI script
} cgistate;

class CGIStrategy : public ResponseBuildingStrategy {
    public:
        CGIStrategy(
            const std::string &location,
            ClientRequest     *_request,
            const std::string &path_info,
            const std::string &cgi_path
        );
        ~CGIStrategy();

        bool build_response();
        bool fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER);
        void set_length(bool len);
        bool get_length() const;
        void save_mem();

    private:
        static Logger babyphone;
        void          fill_env(std::map< std::string, std::string > &env, size_t size);
        char        **generate_env(const std::map< std::string, std::string > &env) const;
        void          init_CGI();
        void          de_chunk();
        void          launch_CGI(size_t size);
        void          feed_CGI();
        void          kill_child(bool k = true);

        std::string    _location;
        std::string    _path_info;
        std::string    _cgi_path;
        std::string    _cgi_response;
        ClientRequest *_request;
        Body          *_body;
        int            _mosi[2]; // master out slave in
        int            _miso[2]; // master in slave out
        pid_t          _child;
        cgistate       _state; ///< When body is ready to be
        bool           _was_dechunked;
        bool           _is_length;
};

#endif // INCLUDE_SRC_CGISTRATEGY_HPP_
