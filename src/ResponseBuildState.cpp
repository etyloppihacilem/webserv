/* #############################################################################

               """          ResponseBuildState.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:12
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ResponseBuildState.hpp"
#include "ProcessState.hpp"

ResponseBuildState::ResponseBuildState(int fd, ClientRequest *request):
    ProcessState(fd),
    _request    (request) {}

ResponseBuildState::~ResponseBuildState() {}
