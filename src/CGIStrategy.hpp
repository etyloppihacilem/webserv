/* #############################################################################

               """          CGIStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 26 Jun. 2024 at 11:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_CGISTRATEGY_HPP_
#define INCLUDE_SRC_CGISTRATEGY_HPP_

#include "ResponseBuildState.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <cstddef>
#include <string>

class CGIStrategy: public ResponseBuildingStrategy {
    public:
        CGIStrategy(ResponseBuildState &state);
        ~CGIStrategy();

        bool        build_response();
        bool        fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER);
        void        save_mem();

    private:
        std::string _location;
};

#endif  // INCLUDE_SRC_CGISTRATEGY_HPP_
