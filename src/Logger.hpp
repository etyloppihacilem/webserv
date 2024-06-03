/* #############################################################################

               """          Logger.hpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 11:13
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_LOGGER_HPP_
#define INCLUDE_SRC_LOGGER_HPP_

#include <cstddef>
#include <fstream>
#include <string>

class Logger {
    public:
        Logger(std::ostream &os, std::string level = "INFO", std::string color = "", size_t width = 0);
        ~Logger();

        void            log(std::string message);
    private:
        std::string     _level;
        std::ofstream   _os;
        int _width;
};

extern Logger   info;
extern Logger   warn;
extern Logger   error;

#endif  // INCLUDE_SRC_LOGGER_HPP_
