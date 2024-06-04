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
#ifdef TESTING
#include "gtest/gtest.h"
#endif

class BodyChunk: public Body {
    public:
        BodyChunk(int fd, std::string &buffer);
        ~BodyChunk();

        std::string &get();
        std::string pop();
        size_t      read_body();

    private:
        size_t      _bytes_remaining;

        size_t      chunk_size();
        bool        is_hex(int c);
#ifdef TESTING
        FRIEND_TEST(BodyLengthTestSuite, Constructor);
#endif
};

#endif  // INCLUDE_SRC_BODYCHUNK_HPP_
