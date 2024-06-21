/* #############################################################################

               """          main.cpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 15:19
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Logger.hpp"
#include "MemoryHandler.hpp"

int main(int ac, char **av) {
    (void) ac;
    (void) av;
    info.log("info message -> %d", 1024);
    info.log() << "info message -> " << 1024 << info.endl();
    warn.log("warn message -> %s", "hihi");
    warn.log() << "warn message -> " << "hihi" << error.endl();
    error.log("error message -> %p", "mais");
    error.log() << "error message -> " << std::ios_base::hex << "mais" << error.endl();
    mem.allocate(); // allocating memory
    return 0;
}
