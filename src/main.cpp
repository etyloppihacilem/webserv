/* #############################################################################

               """          main.cpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 15:19
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Logger.hpp"
#include "ServerManager.hpp"
#include "header.h"
#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <ostream>
#include <signal.h>
#include <string>

volatile sig_atomic_t g_signal = false;

void sigint_handler(int signum) {
    g_signal += true;
    std::cerr << std::endl;
    if (g_signal > true) {
        fatal.log() << "Goodbye..." << std::endl;
        exit(64);
    }
    info.log() << "ServerReactor: JEON stop listening." << std::endl;
    (void) signum;
}

void sigpipe_handler(int signum) {
    (void) signum;
    debug.log() << "Just got sigpipe" << std::endl;
}

static void header() {
    info.log() << "        " << a << std::endl;
    info.log() << "        " << b << std::endl;
    info.log() << "        " << c << std::endl;
    info.log() << "        " << d << std::endl;
    info.log() << "        " << e << std::endl;

    info.log() << "            Just Enough" << std::endl;
    info.log() << "                  Of NGINX" << std::endl;
}

int main(int ac, char **av, const char **env) {
    if (ac != 2) {
        error.log() << "Hint: ./webserv {config_file} " << std::endl;
        return 1;
    }
    Logger::force(); // Forcing all output in normal operations.
#ifndef DEBUG
    debug.disable();
    event.disable();
#endif
    // event.disable();
    header();
    try {
        signal(SIGINT, sigint_handler);
        signal(SIGPIPE, sigpipe_handler);
        ServerManager::getInstance(std::string(av[1]), env);
        ServerManager::getInstance()->run();
        ServerManager::deleteInstance();
        return 0;
    } catch (ServerManager::FailToInitServerError &e) {
        debug.log() << "Exit with FailToInitServerError exception." << std::endl;
        ServerManager::deleteInstance();
        return 2;
    } catch (std::exception &e) {
        error.log() << e.what() << std::endl;
        debug.log() << "Exit with standard exception." << std::endl;
        ServerManager::deleteInstance();
        return 1;
    } catch (...) {
        fatal.log() << "Caught and exit with unexpected error!" << std::endl;
        ServerManager::deleteInstance();
        return 1;
    }
}

/*info.log() << "info message -> " << 1024 << std::endl;*/
/*warn.log() << "warn message -> " << "hihi " << ImATeapot << std::endl;*/
/*error.log() << "error message -> " << (void *) "mais" << std::endl;*/
/*mem.allocate(); // allocating memory*/
/*fatal.log() << std::hex << 42 << std::endl;*/
/*debug.log() << "Super ça manquait de debug" << std::endl;*/
