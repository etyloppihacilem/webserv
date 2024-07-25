/* #############################################################################

               """          ProcessState.hpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 11:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_PROCESSSTATE_HPP_
#define INCLUDE_SRC_PROCESSSTATE_HPP_

typedef enum e_state {
    s_error = -1,   ///< if there is an error (NOT HTTP ERROR !!!!)
    waiting,        ///< waiting for header to exist or to end
    ready,          ///< done
} t_state;

class ProcessState {
    public:
        ProcessState(int fd);
        virtual ~ProcessState() = 0;

        // process returns true when done
        virtual t_state process() = 0;

        t_state         get_state() const;

    protected:
        // TODO:handler manifestement mais osef
        int     _fd;
        t_state _state;
};

#endif  // INCLUDE_SRC_PROCESSSTATE_HPP_
