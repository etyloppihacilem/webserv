/* #############################################################################

               """          Logger.cpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 11:13
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Logger.hpp"
#include "HttpStatusCodes.hpp"
#include "colors.hpp"
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>

Logger::Logger(std::ostream &os, std::string level, std::string color, size_t width) : _enabled(true) {
    _dev_null.setstate(std::ios_base::badbit); // set /dev/null to be /dev/null
    width -= level.length();
    _level = "[ ";
    for (size_t i = 0; i < width; i++)
        _level += " ";
    if (os.rdbuf() == std::cout.rdbuf() || os.rdbuf() == std::cerr.rdbuf())
        _level += color + level + _RESET;
    else
        _level += level;
    _level += " ]";
    _os.copyfmt(os);
    _os.clear(os.rdstate());
    _os.basic_ios< char >::rdbuf(os.rdbuf());
}

Logger::~Logger() {}

/**
  Print a log message. Same syntax as printf, with format string and variadic parameters. Does add a \n at the end.

  Any message have a maximal length of LOG_MAX_SIZE.
  */
// void Logger::log(const char *format, ...) {
//     if (!_enabled && !_force)
//         return;
//     time_t  now = time(0);
//     tm     *ltm = localtime(&now);
//     char    buffer[LOG_MAX_SIZE];
//     va_list args;

// va_start(args, format);
// vsnprintf(buffer, LOG_MAX_SIZE, format, args);
// va_end(args);
// _os << std::dec << std::setw(_width) << std::setfill(' ') << std::left << _level << std::right << std::setw(2)
//     << std::setfill('0') << ltm->tm_hour << ":" << std::setw(2) << std::setfill('0') << ltm->tm_min << ":"
//     << std::setw(2) << std::setfill('0') << ltm->tm_sec << " " << std::setw(2) << std::setfill('0') << ltm->tm_mday
//     << "/" << std::setw(2) << std::setfill('0') << 1 + ltm->tm_mon << "/" << 1900 + ltm->tm_year << ": " << buffer
//     << std::endl;
// }

std::ostream &operator<<(std::ostream &os, const HttpCode code) {
    os << static_cast< int >(code) << " " << status_string(code);
    return os;
}

std::ofstream &Logger::log() {
    if (!_enabled && !_force)
        return _dev_null;
    time_t now = time(0);
    tm    *ltm = localtime(&now);

    _os << std::dec << std::setfill(' ') << std::left << _level << std::right << std::setw(2) << std::setfill('0')
        << ltm->tm_hour << ":" << std::setw(2) << std::setfill('0') << ltm->tm_min << ":" << std::setw(2)
        << std::setfill('0') << ltm->tm_sec << " " << std::setw(2) << std::setfill('0') << ltm->tm_mday << "/"
        << std::setw(2) << std::setfill('0') << 1 + ltm->tm_mon << "/" << 1900 + ltm->tm_year << ": ";
    return _os;
}

void Logger::enable() {
    _enabled = true;
    if (_force)
        log() << "Enabling this log. (force)" << std::endl;
}

void Logger::disable() {
    if (_force)
        log() << "Disabling this log. (force)" << std::endl;
    _enabled = false;
}

bool Logger::is_enabled() {
    if (_force)
        log() << "This log is " << (_enabled ? "enabled" : "disabled") << ". (force)" << std::endl;
    return _enabled;
}

void Logger::force() {
    _force = true;
}

void Logger::unforce() {
    _force = false;
}

bool Logger::is_forced() {
    return _force;
}

std::ofstream Logger::_dev_null;
bool          Logger::_force = false;

Logger info(std::cerr, "INFO", _BLUE, 8);
Logger warn(std::cerr, "WARNING", _YELLOW, 8);
Logger error(std::cerr, "ERROR", _RED, 8);
Logger fatal(std::cerr, "FATAL", _PURPLE _BLINK, 8);
