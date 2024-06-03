/* #############################################################################

               """          BodyChunk.hpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:52
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_BODYCHUNK_HPP_
#define INCLUDE_SRC_BODYCHUNK_HPP_

#include "Body.hpp"

class BodyChunk: public Body {
    public:
        BodyChunk(int fd, std::string &buffer);
        ~BodyChunk();

    private:
};

#endif  // INCLUDE_SRC_BODYCHUNK_HPP_
