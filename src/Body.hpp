/* #############################################################################

               """          Body.hpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:47
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_BODY_HPP_
#define INCLUDE_SRC_BODY_HPP_

#include <cstddef>
#include <string>

class Body {
    public:
        Body(int fd, std::string &buffer);
        virtual ~Body() = 0;

        virtual std::string get() = 0;              // get whole body
        virtual std::string get(size_t size) = 0;   // only size of body
    protected:
        int         _fd;
        std::string _buffer;
};

#endif  // INCLUDE_SRC_BODY_HPP_
