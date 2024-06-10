/* #############################################################################

               """          main.cpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 15:19
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Logger.hpp"
#include "MemoryHandler.hpp"
#include <iostream>

int main(int ac, char **av) {
    (void) ac;
    (void) av;
    info.log("info message -> %d", 1024);
    warn.log("warn message -> %s", "hihi");
    error.log("error message -> %p", "mais");
    mem.allocate(); // allocating memory
    return (0);
}
