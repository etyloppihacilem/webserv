/* #############################################################################

               """          Logger.cpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 11:13
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Logger.hpp"
#include <cstdarg>
#include <iomanip>
#include <iostream>
#include "colors.hpp"

Logger::Logger(std::ostream &os, std::string level, std::string color, size_t width) {
    width   += 3;
    _level  = "[" + level + "]";
    _width  = (width <= _level.length()) ? _level.length() : width;
    if (os.rdbuf() == std::cout.rdbuf() || os.rdbuf() == std::cerr.rdbuf()) { // if log to stdout or stderr enable color
        _width  += color.length() + sizeof(_RESET) - 1;
        _level  = "[" + color + level + _RESET "]";
    }
    _os.copyfmt(os);
    _os.clear(os.rdstate());
    _os.basic_ios<char>::rdbuf(os.rdbuf());
}

Logger::~Logger() {}

/**
  Print a log message. Same syntax as printf, with format string and variadic parameters. Do add a \n at the end.

  Any message have a maximal length of LOG_MAX_SIZE.
  */
void Logger::log(const char *format, ...) {
    time_t  now     = time(0);
    tm      *ltm    = localtime(&now);
    char    buffer[LOG_MAX_SIZE];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, LOG_MAX_SIZE, format, args);
    va_end(args);
    _os << std::setw(_width) << std::setfill(' ') << std::left << _level << std::right << std::setw(2)
        << std::setfill('0') << ltm->tm_hour << ":" << std::setw(2) << std::setfill('0') << ltm->tm_min << ":"
        << std::setw(2) << std::setfill('0') << ltm->tm_sec << " " << std::setw(2) << std::setfill('0')
        << ltm->tm_mday << "/" << std::setw(2) << std::setfill('0') << 1 + ltm->tm_mon << "/" << 1900 + ltm->tm_year
        << ": " << buffer << std::endl;
}

Logger  info(std::cerr, "INFO", _BLUE, 5);
Logger  warn(std::cerr, "WARN", _YELLOW, 5);
Logger  error(std::cerr, "ERROR", _RED, 5);
