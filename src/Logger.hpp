/* #############################################################################

               """          Logger.hpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 11:13
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_LOGGER_HPP_
#define INCLUDE_SRC_LOGGER_HPP_

#include <fstream>
#include <ios>
#include <ostream>
#include <string>

#define LOG_MAX_SIZE 1024

/**
  Object used to log to terminal or file (any std::ostream) log messages with timestamp.

  Do usage of color if in stdout or stderr and enables them automatically.

  Any message have a maximal length of LOG_MAX_SIZE.
  */
class Logger : public std::ios_base {
    public:
        Logger(
            std::ostream    &os,                ///< Out stream
            std::string     level   = "INFO",   ///< Level to display
            std::string     color   = "",       ///< Color ANSI code (no reset, but including ESC)
            size_t          width   = 0         ///< Width to align level
            );
        ~Logger();

        Logger              &log(const char *format, ...);
        friend Logger       &operator<<(Logger &l, const std::string &msg);
        friend Logger       &operator<<(Logger &l, const char *str);
        friend Logger       &operator<<(Logger &l, const int nb);
        friend Logger       &operator<<(Logger &l, const long nb);
        friend Logger       &operator<<(Logger &l, const long long nb);
        friend Logger       &operator<<(Logger &l, const unsigned int nb);
        friend Logger       &operator<<(Logger &l, const unsigned long nb);
        friend Logger       &operator<<(Logger &l, const unsigned long long nb);
        friend Logger       &operator<<(Logger &l, const char nb);
        friend Logger       &operator<<(Logger &l, const void *ptr);
        static std::string  endl(bool crlf = false);
        Logger              &log();

    private:
        std::string         _level; ///< Log level
        std::ofstream       _os;    ///< Out stream
        int                 _width; ///< Width to align level
};

extern Logger   info;
extern Logger   warn;
extern Logger   error;

#endif  // INCLUDE_SRC_LOGGER_HPP_
