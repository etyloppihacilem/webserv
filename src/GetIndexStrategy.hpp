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
#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <cstddef>
#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct dirent dir_item;

/**
  Object to generate an index of a repository.
  */
class GetIndexStrategy: public ResponseBuildingStrategy {
    public:
        GetIndexStrategy(const std::string &location);
        ~GetIndexStrategy();

        bool    build_response();
        bool    fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER);
        void    save_mem();

    private:
        std::string generateLine(char *name, struct stat *st);
        std::string getType(mode_t mode);

        std::string _location;      ///< Location of the directory to scan
        DIR         *_dir;          ///< Pointer on dir
        bool        _init_done;     ///< Tells if header of response is sent.
        bool        _deinit_done;   ///< Tells if footer of response is sent.
};

#endif  // INCLUDE_SRC_GETINDEXSTRATEGY_HPP_
