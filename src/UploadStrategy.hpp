/* #############################################################################

               """          UploadStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 24 Jun. 2024 at 11:40
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_UPLOADSTRATEGY_HPP_
#define INCLUDE_SRC_UPLOADSTRATEGY_HPP_

#include "ClientRequest.hpp"
#include "ResponseBuildingStrategy.hpp"
#include <cstddef>
#include <string>

/**
  Strategy to upload file to server.

  run build_response until file is written.
  */
class UploadStrategy : public ResponseBuildingStrategy {
    public:
        UploadStrategy(ClientRequest &request, std::string location, bool replace = false);
        ~UploadStrategy();

        bool            build_response();
        bool            fill_buffer(std::string &buffer, size_t size);

    private:
        ClientRequest   &_request;
        std::string     _location;
        bool            _replace;
};

#endif  // INCLUDE_SRC_UPLOADSTRATEGY_HPP_
