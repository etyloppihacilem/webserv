/* #############################################################################

               """          main.cpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 15:19
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Logger.hpp"
#include <iostream>

int main(int ac, char **av) {
    (void) ac;
    (void) av;
    info.log("info message");
    warn.log("warn message");
    error.log("error message");
    return (0);
}
