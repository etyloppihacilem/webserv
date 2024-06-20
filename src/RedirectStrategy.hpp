/* #############################################################################

               """          RedirectStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 12 Jun. 2024 at 10:22
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_REDIRECTSTRATEGY_HPP_
#define INCLUDE_SRC_REDIRECTSTRATEGY_HPP_

#include "HttpStatusCodes.hpp"
#include "ResponseBuildingStrategy.hpp"
#include <string>

/**
  Strategy to handle a redirect (3xx code).
  */
class RedirectStrategy : public ResponseBuildingStrategy {
    public:
        RedirectStrategy(
            const std::string   &location,              ///< Location to redirect to
            ResponseBuildState  &state,                 ///< ResponseBuildState
            HttpCode            code = MovedPermanently ///< Code to redirect (MovedPermanently 301 as default)
            );
        ~RedirectStrategy();

        void        buildResponse();
        bool        fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER);

    private:
        std::string _location;
        HttpCode    _code;
};

#endif  // INCLUDE_SRC_REDIRECTSTRATEGY_HPP_
