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
#include "ProcessState.hpp"

// TODO this is supposed to build response some times or another

class ResponseBuildState : ProcessState {
    public:
        ResponseBuildState(int fd, ClientRequest *request);
        ~ResponseBuildState();

    private:
        ClientRequest               *_request;
        ResponseBuildingStrategy    *_strategy;
};

#endif  // INCLUDE_SRC_RESPONSEBUILDSTATE_HPP_
