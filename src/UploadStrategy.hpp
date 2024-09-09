/* #############################################################################

               """          UploadStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 24 Jun. 2024 at 11:40
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_UPLOADSTRATEGY_HPP_
#define INCLUDE_SRC_UPLOADSTRATEGY_HPP_

#include "Body.hpp"
#include "ClientRequest.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "Route.hpp"
#include "Server.hpp"
#include <cstddef>
#include <fstream>
#include <string>

/**
  Strategy to upload file to server.

  run build_response until file is written.
  */
template < class ServerClass = Server, class RouteClass = Route > // template is there for testing purposes
class UploadStrategy : public ResponseBuildingStrategy {
    public:
        UploadStrategy(
            ClientRequest     &request,
            const std::string &location,
            const ServerClass &server,
            bool               diff,
            size_t             max_size,
            bool               replace = false
        );
        ~UploadStrategy();

        bool build_response();
        bool fill_buffer(std::string &buffer, size_t size);

    private:
        void        init();
        void        init_location();
        std::string create_name(int nb);

        bool               _init;
        const ServerClass &_server;
        std::fstream       _file;
        Body              *_body;
        std::string        _target;
        std::string        _location;
        size_t             _max_size;
        bool               _replace;
        bool               _diff; ///< Upload path different than root dir
#ifdef TESTING
# include "gtest/gtest.h"
        FRIEND_TEST(ResponseBuildStateFixture, CorrectStrategyTest);
#endif
};

#endif // INCLUDE_SRC_UPLOADSTRATEGY_HPP_
