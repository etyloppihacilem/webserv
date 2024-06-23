/* #############################################################################

               """          Logger.hpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 11:13
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_LOGGER_HPP_
#define INCLUDE_SRC_LOGGER_HPP_

#include "HttpStatusCodes.hpp"
#include <fstream>
#include <ios>
#include <istream>
#include <ostream>
#include <string>

#define LOG_MAX_SIZE 1024

/**
  Object used to log to terminal or file (any std::ostream) log messages with timestamp.

  Do usage of color if in stdout or stderr and enables them automatically.

  Any message have a maximal length of LOG_MAX_SIZE.
  */
class Logger : public std::ostream {
    public:
        Logger(
            std::ostream    &os,                ///< Out stream
            std::string     level   = "INFO",   ///< Level to display
            std::string     color   = "",       ///< Color ANSI code (no reset, but including ESC)
            size_t          width   = 0         ///< Width to align level
            );
        ~Logger();

        Logger              &log(const char *format, ...);
        static std::string  endl(bool crlf = false);
        std::ofstream       &log();

    private:
        std::string         _level;         ///< Log level
        std::ofstream       _os;            ///< Out stream
        int                 _width;         ///< Width to align level
};

std::ostream &operator<<(std::ostream &os, const HttpCode code);

extern Logger   info;
extern Logger   warn;
extern Logger   error;

#endif  // INCLUDE_SRC_LOGGER_HPP_
