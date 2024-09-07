/* #####################################################################################################################

               """          CGIHandlerMISO.hpp
        -\-    _|__
         |\___/  . \        Created on 01 Sep. 2024 at 21:23
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_SRC_CGIHANDLERMISO_HPP_
#define INCLUDE_SRC_CGIHANDLERMISO_HPP_

#include "CGIWriter.hpp"
#include "EventHandler.hpp"
#include <ctime>
#include <string>

class CGIStrategy;

class CGIHandlerMISO : public EventHandler {
    public:
        CGIHandlerMISO(int MISO_fd, CGIStrategy &strategy, CGIWriter &writer, const std::string &temp_file);
        ~CGIHandlerMISO();

        time_t getTimeout() const;
        void   handle(); // Run this in loop
        void   timeout();

    private:
        CGIStrategy &_strategy;
        CGIWriter   &_writer;
        std::string _temp_file;
};

#endif // INCLUDE_SRC_CGIHANDLERMISO_HPP_
