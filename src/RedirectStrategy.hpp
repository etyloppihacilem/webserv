/* #############################################################################

               """          RedirectStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 12 Jun. 2024 at 10:22
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_REDIRECTSTRATEGY_HPP_
#define INCLUDE_SRC_REDIRECTSTRATEGY_HPP_

#include "ResponseBuildingStrategy.hpp"
#include <string>

class RedirectStrategy : public ResponseBuildingStrategy {
    public:
        RedirectStrategy(const std::string &location, ResponseBuildState &state);
        ~RedirectStrategy();

        void        buildResponse();
        bool        fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER);

    private:
        std::string _location;
};

#endif  // INCLUDE_SRC_REDIRECTSTRATEGY_HPP_
