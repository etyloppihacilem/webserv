#include "AcceptHandler.hpp"
#include "EventHandler.hpp"
#include <string>

AcceptHandler::AcceptHandler(int socket, int port) : EventHandler(socket, port) {
    int toto = 0;
    (void) toto;
}

AcceptHandler::~AcceptHandler() {}

void AcceptHandler::handle() {
    std::string do_stuff;
    (void)do_stuff;
}
