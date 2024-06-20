/* #############################################################################

               """          ReadState.hpp
        -\-    _|__
         |\___/  . \        Created on 01 Jun. 2024 at 14:23
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_READSTATE_HPP_
#define INCLUDE_SRC_READSTATE_HPP_

#include "ClientRequest.hpp"
#include "ProcessState.hpp"
#include <cstddef>
#include <string>

typedef enum e_state {
    error = -1, ///< if there is an error
    waiting,    ///< waiting for header to exist or to end
    ready,      ///< done
    ready_body, ///< done but body exists TODO check if used
} t_state;

class ReadState : public ProcessState {
    public:
        ReadState(int fd);
        ~ReadState();

        bool            process();
        t_state         process_buffer(char *buffer);
        // TODO is a function to check header requirements needed ?? For example host requirements (that are checked)
        ClientRequest   *get_message();
        void            done_message();

    private:
        t_state         _state;         ///< State of the object
        std::string     _buffer;        ///< Buffer for parsing on fd
        /**< buffer is supposed clean at the end of a successful parsing on it. */
        ClientRequest   *_in_progress;  ///< ClientRequest that is built

        size_t          find_method();
#ifdef TESTING
        FRIEND_TEST(ReadStateSuite, FindMethod);
#endif
};

#endif  // INCLUDE_SRC_READSTATE_HPP_
