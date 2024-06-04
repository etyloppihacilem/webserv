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

class BodyLength : public Body {
    public:
        BodyLength(int fd, std::string &buffer, std::string length);
        ~BodyLength();

        std::string &get();
        std::string pop();
        size_t      read_body();
    private:
        size_t      _length;
        size_t      _read_length;
#ifdef TESTING
        FRIEND_TEST(BodyLengthTestSuite,    Constructor);
        FRIEND_TEST(BodyLengthTestSuite,    BadConstructor);
        FRIEND_TEST(BodyLengthTestSuite,    read_body);
#endif
};

#endif  // INCLUDE_SRC_BODYLENGTH_HPP_
