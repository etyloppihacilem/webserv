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
#include <ios>
#include <ostream>

//TODO: add signal handling for SIGINT(interrupt) and SIGPIPE(ignore)
int main(int ac, char **av) {
    (void) ac;
    (void) av;
    Logger::force(); // Forcing all output in normal operations.
#ifndef DEBUG
    debug.disable();
#endif
    info.log() << "info message -> " << 1024 << std::endl;
    warn.log() << "warn message -> " << "hihi " << ImATeapot << std::endl;
    error.log() << "error message -> " << (void *) "mais" << std::endl;
    mem.allocate(); // allocating memory
    fatal.log() << std::hex << 42 << std::endl;
    debug.log() << "Super ça manquait de debug" << std::endl;
    return 0;
}
