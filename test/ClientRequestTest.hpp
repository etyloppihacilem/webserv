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
#include "ClientRequest_test.hpp"
#include "gtest/gtest.h"

class ClientRequestTestTarget : public testing::TestWithParam<t_test_target> {
    public:
        ClientRequestTestTarget() : test(0) {}

    protected:
        ClientRequest test;
};

class ClientRequestTestParseHeader : public testing::TestWithParam<t_test_target> {
    public:
        ClientRequestTestParseHeader() : test(0) {}

    protected:
        ClientRequest test;
};

class ClientRequestTestInitHeader : public testing::TestWithParam<s_test_messages> {
    public:
        ClientRequestTestInitHeader() : test(0) {}

    protected:
        ClientRequest test;
};

#endif // INCLUDE_SRC_MESSAGETEST_HPP_
