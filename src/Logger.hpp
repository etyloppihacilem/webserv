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
            std::ostream &os,             ///< Out stream
            std::string   level = "INFO", ///< Level to display
            std::string   color = "",     ///< Color ANSI code (no reset, but including ESC)
            size_t        width = 0       ///< Width to align level
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
        std::string          _level;    ///< Log level
        std::ofstream        _os;       ///< Out stream
        int                  _width;    ///< Width to align level
        bool                 _enabled;  ///< Tells if test is enabled
};

std::ostream &operator<<(std::ostream &os, const HttpCode code);

extern Logger info;
extern Logger warn;
extern Logger error;

#endif // INCLUDE_SRC_LOGGER_HPP_
