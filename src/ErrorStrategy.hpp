/* #############################################################################

               """          ErrorStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 23 Jun. 2024 at 16:58
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_ERRORSTRATEGY_HPP_
#define INCLUDE_SRC_ERRORSTRATEGY_HPP_

#include "HttpStatusCodes.hpp"
#include "ResponseBuildingStrategy.hpp"

/**
  Strategy to send an error.

  Recovery will not use personalized error page but will generate one.
  */
class ErrorStrategy: public ResponseBuildingStrategy {
    public:
        ErrorStrategy(ResponseBuildState &state, HttpCode code = InternalServerError, bool recovery = true);
        // TODO trouver comment recuperer la map des fichiers d'erreures.
        ~ErrorStrategy();

        void        buildResponse();
        bool        fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER);
        void        save_mem();

    private:
        void        generateErrorPage(std::string &buffer);
        HttpCode    _code;
        bool        _recovery;
};

#endif  // INCLUDE_SRC_ERRORSTRATEGY_HPP_
