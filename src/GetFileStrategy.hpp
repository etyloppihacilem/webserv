/* #############################################################################

               """          GetFileStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 19 Jun. 2024 at 11:25
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_GETFILESTRATEGY_HPP_
#define INCLUDE_SRC_GETFILESTRATEGY_HPP_

#include "HttpStatusCodes.hpp"
#include "MimeTypes.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <cstddef>
#include <fstream>
#include <string>

class GetFileStrategy : public ResponseBuildingStrategy {
    public:
        GetFileStrategy(const MimeTypes &mime, const std::string &location, HttpCode code = OK);
        ~GetFileStrategy();

        bool build_response();
        bool fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER);
        void save_mem();

    private:
        const MimeTypes &_mime;
        std::string      _location;
        HttpCode         _code;
        std::fstream     _file;
};

#endif // INCLUDE_SRC_GETFILESTRATEGY_HPP_
