/* #############################################################################

               """          GetIndexStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 14:11
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_GETINDEXSTRATEGY_HPP_
#define INCLUDE_SRC_GETINDEXSTRATEGY_HPP_

#include "BodyWriter.hpp"
#include "ResponseBuildState.hpp"
#include "ResponseBuildingStrategy.hpp"
#include <cstddef>
#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct dirent dir_item;

class GetIndexStrategy: public ResponseBuildingStrategy {
    public:
        GetIndexStrategy(const std::string &location, ResponseBuildState &state);
        ~GetIndexStrategy();

        void        buildResponse();
        bool        fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER);

    private:
        std::string generateLine(char *name, struct stat *st);
        std::string getType(mode_t mode);

        std::string _location;
        DIR         *_dir;
        bool        _init_done;
        bool        _deinit_done;
};

#endif  // INCLUDE_SRC_GETINDEXSTRATEGY_HPP_
