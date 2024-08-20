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
#include <string>

class ReadState : public ProcessState {
    public:
        ReadState(int socket);
        ~ReadState();

        t_state process();
        void    save_mem();

        ClientRequest *get_client_request();

    private:
        t_state process_buffer(char *buffer);
        t_state return_error();

        std::string    _buffer; ///< Buffer for parsing on socket
        /**< buffer is supposed clean at the end of a successful parsing on it. */
        ClientRequest *_in_progress; ///< ClientRequest that is built

#ifdef TESTING
        FRIEND_TEST(ReadStateSuite, FindMethod);
        FRIEND_TEST(TotalRequestFixture, FdStateTest);
#endif
};

#endif // INCLUDE_SRC_READSTATE_HPP_
