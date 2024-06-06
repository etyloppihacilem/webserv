/* #############################################################################

               """          ClientRequestTest.hpp
        -\-    _|__
         |\___/  . \        Created on 09 May. 2024 at 15:17
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_MESSAGETEST_HPP_
#define INCLUDE_SRC_MESSAGETEST_HPP_

#include "ClientRequest.hpp"
#include "gtest/gtest.h"
#include <string>
#include <vector>
#include "ClientRequest_test.hpp"

class ClientRequestTestTarget: public testing::TestWithParam<t_test_target> {
    public:
    protected:
        ClientRequest test;
};

class ClientRequestTestParseHeader: public testing::TestWithParam<t_test_target> {
    public:
    protected:
        ClientRequest test;
};

class ClientRequestTestInitHeader: public testing::TestWithParam<s_test_messages> {
    public:
    protected:
        ClientRequest test;
};

#endif  // INCLUDE_SRC_MESSAGETEST_HPP_
