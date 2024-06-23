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
#include "MimeTypes.hpp"
#include <ios>
#include <ostream>

int main(int ac, char **av) {
    (void) ac;
    (void) av;
    info.log("info message -> %d", 1024);
    warn.log("warn message -> %s %d %s", "hihi", ImATeapot, status_string(ImATeapot).c_str());
    error.log("error message -> %p", "mais");
    info.log() << "info message -> " << 1024 << std::endl;
    warn.log() << "warn message -> " << "hihi " << ImATeapot << std::endl;
    error.log() << "error message -> " << (void *) "mais" << std::endl;
    mem.allocate(); // allocating memory
    info.log() << std::hex << 42 << std::endl;
    return 0;
}
