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

class ReadState : public ProcessState {
    public:
        ReadState(int fd);
        ~ReadState();

        t_state         process();
        t_state         process_buffer(char *buffer);
        ClientRequest   *get_client_request();
        t_state         return_error();
        void    done_client_request();
        void    save_mem();

    private:
        size_t          find_method();

        std::string     _buffer;           ///< Buffer for parsing on fd
        /**< buffer is supposed clean at the end of a successful parsing on it. */
        ClientRequest   *_in_progress;  ///< ClientRequest that is built

#ifdef TESTING
        FRIEND_TEST(ReadStateSuite,         FindMethod);
        FRIEND_TEST(TotalRequestFixture,    FdStateTest);
#endif
};

#endif  // INCLUDE_SRC_READSTATE_HPP_
