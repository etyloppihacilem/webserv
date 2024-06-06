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
    error = -1,
    waiting,    // waiting for header to exist or to end
    ready,
    ready_body, // waiting for body to arrive or to end
} t_state;

class ReadState : public ProcessState {
    public:
        ReadState(int fd);
        ~ReadState();

        bool            process();
        t_state         process_buffer(char *buffer);
        // TODO is a function to check header requirements needed ??
        ClientRequest   &get_message(); // when message is ready to process
        void            done_message(); // when message is done process, to free

    private:
        t_state         _state;
        std::string     _buffer;        // buffer is supposed clean at the end of a successful parsing on it.
        ClientRequest   *_ready;
        ClientRequest   *_in_progress;

        size_t          find_method();
#ifdef TESTING
        FRIEND_TEST(ReadStateSuite, FindMethod);
#endif
};

#endif  // INCLUDE_SRC_READSTATE_HPP_
