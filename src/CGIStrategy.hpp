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
#include "CGIWriter.hpp"
#include "ClientRequest.hpp"
#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <cstddef>
#include <fstream>
#include <map>
#include <sched.h>
#include <string>
#include <time.h>

typedef enum s_cgistate {
    init = 0,     // first init, run only one time
    loading_body, // if body is length so reading it
    launch,       // launch CGI
    launched,
} cgistate;

class CGIStrategy : public ResponseBuildingStrategy {
    public:
        CGIStrategy(
            const std::string &location,
            ClientRequest     *_request,
            const std::string &path_info,
            const std::string &cgi_path,
            size_t             max_size
        );
        ~CGIStrategy();

        bool  build_response();
        bool  fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER);
        void  set_length(bool len);
        void  is_done_building();
        bool  get_length() const;
        pid_t get_child_pid() const;
        void  save_mem();
        bool  is_child_alive();
        bool  open_child_file();

    private:
        static Logger babyphone;
        void          fill_env(std::map< std::string, std::string > &env, size_t size);
        char        **generate_env(const std::map< std::string, std::string > &env) const;
        void          init_CGI();
        void          fill_temp_file();
        void          launch_CGI(size_t size);
        void          kill_child();
        // void          clean_filestream();

        std::string    _location;
        std::string    _path_info;
        std::string    _cgi_path;
        std::string    _temp_file_mosi;
        std::string    _temp_file_miso;
        std::fstream   _temp_stream_mosi;
        // std::fstream   _temp_stream_miso;
        int            _fd_miso;
        ClientRequest *_request;
        Body          *_body;
        pid_t          _child;
        cgistate       _state; ///< When body is ready to be
        bool           _is_length;
        size_t         _max_size;
        CGIWriter     *_writer;
        HttpCode       _code;
};

#endif // INCLUDE_SRC_CGISTRATEGY_HPP_
