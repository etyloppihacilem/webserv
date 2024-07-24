/* #############################################################################

               """          ProcessState.hpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 11:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_PROCESSSTATE_HPP_
#define INCLUDE_SRC_PROCESSSTATE_HPP_

class ProcessState {
    public:
        ProcessState(int fd);
        virtual ~ProcessState() = 0;

        // process returns true when done
        virtual bool process() = 0;

    protected:
        // TODO:handler manifestement mais osef
        int _fd;
};

#endif  // INCLUDE_SRC_PROCESSSTATE_HPP_
