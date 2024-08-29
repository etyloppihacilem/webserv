/* #############################################################################

               """          main.cpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 15:19
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "HttpStatusCodes.hpp"
#include "Logger.hpp"
#include "MemoryHandler.hpp"
#include "ServerManager.hpp"
#include <csignal>
#include <exception>
#include <ios>
#include <iostream>
#include <ostream>
#include <signal.h>
#include <string>

volatile sig_atomic_t g_signal = false;

void sigint_handler(int signum) {
    g_signal = true;
    std::cerr << std::endl;
    info.log() << "ServerReactor: JEON stop listening." << std::endl;
    (void) signum;
}

void sigpipe_handler(int signum) {
    (void) signum;
}

int main(int ac, char **av) {
    if (ac != 2) {
        error.log() << "Hint: ./webserv {config_file} " << std::endl;
        return 1;
    }
    Logger::force(); // Forcing all output in normal operations.
#ifndef DEBUG
    debug.disable();
#endif
    try {
        signal(SIGINT, sigint_handler);
        signal(SIGPIPE, sigpipe_handler);
        ServerManager::getInstance(std::string(av[1]));
        ServerManager::getInstance()->run();
        ServerManager::deleteInstance();
        return 0;
    } catch (ServerManager::FailToInitServerError &e) {
        return 2;
    } catch (std::exception &e) {
        return 1;
    } catch (...) {
        fatal.log() << "Caught unexpected error!" << std::endl;
        return 1;
    }
}

/*info.log() << "info message -> " << 1024 << std::endl;*/
/*warn.log() << "warn message -> " << "hihi " << ImATeapot << std::endl;*/
/*error.log() << "error message -> " << (void *) "mais" << std::endl;*/
/*mem.allocate(); // allocating memory*/
/*fatal.log() << std::hex << 42 << std::endl;*/
/*debug.log() << "Super ça manquait de debug" << std::endl;*/
