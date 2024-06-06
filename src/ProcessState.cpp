/* #############################################################################

               """          ProcessState.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 11:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ProcessState.hpp"

ProcessState::ProcessState(int fd):
    _fd(fd) {}

ProcessState::~ProcessState() {}
