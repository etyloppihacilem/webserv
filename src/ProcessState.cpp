/* #############################################################################

               """          ProcessState.cpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 11:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ProcessState.hpp"

ProcessState::ProcessState(int fd):
    _fd(fd),
    _state(waiting) ///< State of the object
{}

ProcessState::~ProcessState() {}

t_state ProcessState::get_state() const {
    return _state;
}
