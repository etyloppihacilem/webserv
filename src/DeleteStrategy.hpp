/* #############################################################################

               """          DeleteStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 25 Jun. 2024 at 12:22
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_DELETESTRATEGY_HPP_
#define INCLUDE_SRC_DELETESTRATEGY_HPP_

#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <cstddef>
#include <string>

/**
  Strategy to delete a file.
  */
class DeleteStrategy: public ResponseBuildingStrategy {
    public:
        DeleteStrategy(const std::string &location);
        ~DeleteStrategy();

        bool        build_response();
        bool        fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER);
        void        save_mem();

    private:
        std::string _location;
};

#endif  // INCLUDE_SRC_DELETESTRATEGY_HPP_
