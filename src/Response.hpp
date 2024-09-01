/* #############################################################################

               """          Response.hpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:47
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_RESPONSE_HPP_
#define INCLUDE_SRC_RESPONSE_HPP_

#include "BodyWriter.hpp"
#include "HttpStatusCodes.hpp"
#include "ProcessState.hpp"
#include <cstddef>
#include <ctime>
#include <map>
#include <string>

#ifdef TESTING
# include "gtest/gtest.h"
#endif

typedef std::map< std::string, std::string >::const_iterator mapit;

/**
  Response object. This contains everything needed to store and generate a response header and body.
  */
class Response {
    public:
        Response();
        ~Response();

        void        set_body(const std::string &body_content, std::string content_type = "text/plain");
        void        set_body(ResponseBuildingStrategy *strategy);
        void        set_cgi(ResponseBuildingStrategy *strategy);
        ///< To tell response to process CGI reading when bodywriter is called
        void        add_header(const std::string &field, const std::string &value);
        void        set_code(HttpCode code);
        void        set_code(std::string code); // not a reference
        HttpCode    get_code() const;
        bool        build_response(std::string &buffer, size_t size);
        bool        is_done() const;
        void        save_mem();
        BodyWriter *get_body();
        bool        have_body();
        void        date(); ///< Sets Date header to current time, and sets last modified if defined
        void        set_last_modified(const struct timespec &time);
        void set_head(bool is_head);

    private:
        std::string generate_status_line() const;
        std::string generate_header() const;
        void        clean_body();

        HttpCode                             _code;   ///< HttpStatusCodes of response
        std::map< std::string, std::string > _header; ///< Header map
        BodyWriter                          *_body;   ///< Body of response (if any)
        internal_state                       _state;  ///< To know what is left to generate
        struct timespec                      _last_modified;
        bool                                 _is_head;
#ifdef TESTING
        FRIEND_TEST(ResponseTestSuite, generate_status_line);
        FRIEND_TEST(ResponseBuildStateFixture, CorrectHeaders);
        FRIEND_TEST(ResponseBuildStateFixture, BodyValue);
#endif
};

#endif // INCLUDE_SRC_RESPONSE_HPP_
