/* #############################################################################

               """          ResponseBuildingStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:39
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_RESPONDEBUILDINGSTRATEGY_CPP_
#define INCLUDE_SRC_RESPONDEBUILDINGSTRATEGY_CPP_

#include "Response.hpp"
#include "ResponseBuildState.hpp"

class ResponseBuildingStrategy {
    public:
        ResponseBuildingStrategy(ResponseBuildState &state);
        ~ResponseBuildingStrategy();

        virtual void        buildResponse() = 0;
        Response            &get_response();
        bool is_done();

    private:
        ResponseBuildState  &_state;
        Response            _response;
        bool                _done;
};

#endif  // INCLUDE_SRC_RESPONDEBUILDINGSTRATEGY_CPP_
