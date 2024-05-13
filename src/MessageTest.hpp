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
#include "Message_test.hpp"

class MessageTestTarget: public testing::TestWithParam<t_test_target> {
    public:
    protected:
        Message test;
};

class MessageTestParseHeader: public testing::TestWithParam<t_test_target> {
    public:
    protected:
        Message test;
};

class MessageTestInitHeader: public testing::TestWithParam<s_test_messages> {
    public:
    protected:
        Message test;
};

#endif  // INCLUDE_SRC_MESSAGETEST_HPP_
