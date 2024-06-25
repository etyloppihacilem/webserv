/* #############################################################################

               """          ResponseBuildState.hpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:11
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_RESPONSEBUILDSTATE_HPP_
#define INCLUDE_SRC_RESPONSEBUILDSTATE_HPP_

#include "BodyWriter.hpp"
#include "ClientRequest.hpp"
#include "HttpStatusCodes.hpp"
#include "ProcessState.hpp"

// TODO this is supposed to build response some times or another

/**
  This class is in charge of initiating the right strategy for the given ClientRequest and generating the response.
  It will be done when the response will be built.
  */
class ResponseBuildState : public ProcessState {
    public:
        ResponseBuildState(int fd, ClientRequest *request);
        ~ResponseBuildState();

        bool                        process();
        ClientRequest               *get_request();
        ResponseBuildingStrategy    *get_response_strategy();

    private:
        void                        init_strategy();
        void                        init_strategy(HttpCode code);

        ClientRequest               *_request;
        ResponseBuildingStrategy    *_strategy;
        bool                        _was_error; ///< Tells if last strategy was an error to enable recovery if needed.
};

#endif  // INCLUDE_SRC_RESPONSEBUILDSTATE_HPP_
