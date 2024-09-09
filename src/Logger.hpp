/* #############################################################################

               """          Logger.hpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 11:13
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_LOGGER_HPP_
#define INCLUDE_SRC_LOGGER_HPP_

#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#include <cstddef>
#include <fstream>
#include <ostream>
#include <string>

// #define LOG_MAX_SIZE 1024

/**
  Object used to log to terminal or file (any std::ostream) log messages with timestamp.

  Do usage of color if in stdout or stderr and enables them automatically.

  Any message have a maximal length of LOG_MAX_SIZE.
  */
class Logger : public std::ostream {
    public:
        Logger(
            std::ostream &os,                ///< Out stream
            std::string   level    = "INFO", ///< Level to display
            std::string   color    = "",     ///< Color ANSI code (no reset, but including ESC)
            size_t        width    = 0,      ///< Width to align level
            bool          no_force = false   ///< Remove force option for this log stream
        );
        Logger(
            const std::string &filename,          ///< Out file
            std::string        level    = "FILE", ///< Level to display
            size_t             width    = 0,      ///< Width to align level
            bool               no_force = false   ///< Remove force option for this log stream
        );
        ~Logger();

        // void               log(const char *format, ...);
        static std::string endl(bool crlf = false);
        std::ofstream     &log();
        void               enable(); ///< Function use to enable or disable test output
        void               disable();
        bool               is_enabled();
        static void        force();     ///< Function to force log output even if disabled
        static void        unforce();   ///< Remove force
        static bool        is_forced(); ///< Remove force

    private:
        static std::ofstream _dev_null; ///< dev_null for disable
        static bool          _force;    ///< force log even if disabled
        std::string          _level;    ///< Log level"
        std::ofstream        _os;       ///< Out stream
        std::fstream         _file;
        bool                 _enabled;  ///< Tells if test is enabled
        bool                 _no_force; ///< force log even if disabled
        bool                 _is_file;
};

std::ostream &operator<<(std::ostream &os, const HttpCode code);
std::ostream &operator<<(std::ostream &os, const HttpMethod method);

extern Logger info;
extern Logger warn;
extern Logger error;
extern Logger fatal;
extern Logger debug;
extern Logger event;

#endif // INCLUDE_SRC_LOGGER_HPP_
