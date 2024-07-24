/* #############################################################################

               """          BodyLength.hpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:52
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_BODYLENGTH_HPP_
#define INCLUDE_SRC_BODYLENGTH_HPP_

#include <cstddef>
#include <string>

#ifdef TESTING
#include "gtest/gtest.h"
#endif

#include "Body.hpp"

/**
  BodyLength is mean to read body sent with the Content-Length header.

  There is no particular body structure.
  */
class BodyLength : public Body {
    public:
        BodyLength(int fd, std::string &buffer, std::string length /** is a string representing a decimal. */ );
        ~BodyLength();

        std::string &get();
        std::string pop();
        void        clean();
        size_t      read_body();

    private:
        size_t  _length;            ///< Length to read
        size_t  _read_length;       ///< Length read from socket.

#ifdef TESTING
        FRIEND_TEST(BodyLengthTestSuite,    Constructor);
        FRIEND_TEST(BodyLengthTestSuite,    BadConstructor);
        FRIEND_TEST(BodyLengthTestSuite,    read_body);
        FRIEND_TEST(BodyLengthTestSuite,    get);
        FRIEND_TEST(BodyLengthTestSuite,    pop);
#endif
};

#endif  // INCLUDE_SRC_BODYLENGTH_HPP_
