/* #############################################################################

               """          ResponseBuildState.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:12
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ResponseBuildState.hpp"
#include "ProcessState.hpp"

/**
 This class is in charge of choosing the right strategy depending on the request.
 */
ResponseBuildState::ResponseBuildState(int fd, ClientRequest *request):
    ProcessState(fd),
    _request    (request),
    _strategy   (0) {   // TODO check where to free this once it is allocated
    (void) _request;    // TODO delete this
    (void) _strategy;   // TODO delete this
}

ResponseBuildState::~ResponseBuildState() {}
