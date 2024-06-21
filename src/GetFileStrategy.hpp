/* #############################################################################

               """          GetFileStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 19 Jun. 2024 at 11:25
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_GETFILESTRATEGY_HPP_
#define INCLUDE_SRC_GETFILESTRATEGY_HPP_

#include "ResponseBuildingStrategy.hpp"
#include <fstream>
#include <string>

class GetFileStrategy : public ResponseBuildingStrategy {
    public:
        GetFileStrategy(std::string &location, ResponseBuildingStrategy &state);
        ~GetFileStrategy();

        void            buildResponse();
        bool            fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER);
        void            save_mem();

    private:
        void            set_mime_type();

        std::string     _location;
        std::fstream    _file;
};

#endif  // INCLUDE_SRC_GETFILESTRATEGY_HPP_
