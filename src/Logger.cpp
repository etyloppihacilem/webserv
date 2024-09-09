/* #############################################################################

               """          Logger.cpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 11:13
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "Logger.hpp"
#include "HttpMethods.hpp"
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

Logger::Logger(std::ostream &os, std::string level, std::string color, size_t width, bool no_force) :
    _enabled(true),
    _no_force(no_force),
    _is_file(false) {
    _dev_null.setstate(std::ios_base::badbit); // set /dev/null to be /dev/null
    width -= level.length();
    if (os.rdbuf() == std::cout.rdbuf() || os.rdbuf() == std::cerr.rdbuf())
        _level = color + "[ ";
    else
        _level = "[ ";
    for (size_t i = 0; i < width; i++)
        _level += " ";
    _level += level;
    if (os.rdbuf() == std::cout.rdbuf() || os.rdbuf() == std::cerr.rdbuf())
        _level += " ]" _RESET;
    else
        _level += " ]";
    _os.copyfmt(os);
    _os.clear(os.rdstate());
    _os.basic_ios< char >::rdbuf(os.rdbuf());
}

Logger::Logger(const std::string &filename, std::string level, size_t width, bool no_force) :
    _enabled(true),
    _no_force(no_force),
    _is_file(false) {
    _dev_null.setstate(std::ios_base::badbit); // set /dev/null to be /dev/null
    width -= level.length();
    _level = "[ ";
    for (size_t i = 0; i < width; i++)
        _level += " ";
    _level += level;
    _level += " ]";
    _file.open(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
    if (_file.is_open()) {
        _is_file = true;
        _os.copyfmt(_file);
        _os.clear(_file.rdstate());
        _os.basic_ios< char >::rdbuf(_file.rdbuf());
    } else {
        _os.copyfmt(std::cerr);
        _os.clear(std::cerr.rdstate());
        _os.basic_ios< char >::rdbuf(std::cerr.rdbuf());
    }
}

Logger::~Logger() {
    if (_is_file)
        _file.close();
}

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

std::ostream &operator<<(std::ostream &os, const HttpMethod method) {
    os << method_string(method);
    return os;
}

std::ofstream &Logger::log() {
    if (!_enabled && !(_force && !_no_force))
        return _dev_null;
    time_t now = time(0);
    tm    *ltm = localtime(&now);

    _os << _level << " " << std::right << std::setw(2) << std::setfill('0') << ltm->tm_hour << ":" << std::setw(2)
        << std::setfill('0') << ltm->tm_min << ":" << std::setw(2) << std::setfill('0') << ltm->tm_sec << " "
        << std::setw(2) << std::setfill('0') << ltm->tm_mday << "/" << std::setw(2) << std::setfill('0')
        << 1 + ltm->tm_mon << "/" << 1900 + ltm->tm_year << ": ";
    return _os;
}

void Logger::enable() {
    if ((_force && !_no_force) && !_enabled)
        log() << "Enabling this log. (force)" << std::endl;
    _enabled = true;
}

void Logger::disable() {
    if ((_force && !_no_force) && _enabled)
        log() << "Disabling this log. (force)" << std::endl;
    _enabled = false;
}

bool Logger::is_enabled() {
    if (_force && !_no_force)
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
Logger debug(std::cerr, "DEBUG", _GREEN, 8, true); // Cannot be forced
Logger event(std::cerr, "EVENT", _CYAN, 8, true); // Cannot be forced
