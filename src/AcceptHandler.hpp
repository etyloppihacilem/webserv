#ifndef INCLUDE_SRC_ACCEPTHANDLER_HPP_
#define INCLUDE_SRC_ACCEPTHANDLER_HPP_

#include "EventHandler.hpp"
#include <ctime>

#define SERVER_TIMEOUT std::numericlimits<time_t>::max()

class AcceptHandler : public EventHandler {
    public:
        AcceptHandler(int socket, int port);
        ~AcceptHandler();

        time_t getTimeout() const;

        void handle();
        void timeout();

};

#endif // !INCLUDE_SRC_ACCEPTHANDLER_HPP_
