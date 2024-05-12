/* #############################################################################

               """          MessageTest.hpp
        -\-    _|__
         |\___/  . \        Created on 09 May. 2024 at 15:17
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_MESSAGETEST_HPP_
#define INCLUDE_SRC_MESSAGETEST_HPP_

#include "Message.hpp"
#include "gtest/gtest.h"
#include <string>
#include <vector>

typedef struct s_test_target {
    std::string name;
    std::string c1;
    std::string c2;
    std::string c3;
} t_test_target;

class MessageTest: public testing::TestWithParam<t_test_target> {
    public:
    protected:
        Message test;
};

class MessageTestParseHeader: public testing::TestWithParam<t_test_target> {
    public:
    protected:
        Message test;
};

#endif  // INCLUDE_SRC_MESSAGETEST_HPP_
