#ifndef INCLUDE_SRC_ACCEPTHANDLER_HPP_
#define INCLUDE_SRC_ACCEPTHANDLER_HPP_

#include "EventHandler.hpp"

class AcceptHandler : public EventHandler {
    public:
    AcceptHandler(int socket, int port);
    ~AcceptHandler();

    void handle();
};

#endif // !INCLUDE_SRC_ACCEPTHANDLER_HPP_
