/* #####################################################################################################################

               """          CGIHandlerMOSI.hpp
        -\-    _|__
         |\___/  . \        Created on 01 Sep. 2024 at 21:23
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_SRC_CGIHANDLERMOSI_HPP_
#define INCLUDE_SRC_CGIHANDLERMOSI_HPP_

#include "EventHandler.hpp"
#include <ctime>

class CGIStrategy;

class CGIHandlerMOSI : public EventHandler {
    public:
        CGIHandlerMOSI(int MOSI_fd, CGIStrategy &strategy);
        ~CGIHandlerMOSI();

        time_t getTimeout() const;
        void   handle(); // Run this in loop
        void   timeout();

    private:
        CGIStrategy &_strategy;
};

#endif // INCLUDE_SRC_CGIHANDLERMOSI_HPP_
