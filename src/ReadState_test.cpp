/* #############################################################################

               """          ReadState_test.cpp
        -\-    _|__
         |\___/  . \        Created on 01 Jun. 2024 at 15:02
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "ReadState.hpp"
#include "gtest/gtest.h"
#include <cstddef>

TEST(ReadStateSuite, FindMethod) {
    ReadState test;

    test._buffer = "GET coucou je suis heureux";
    EXPECT_EQ(test.find_method(), (size_t) 0);
    test._buffer = "POST coucou je suis heureux";
    EXPECT_EQ(test.find_method(), (size_t) 0);
    test._buffer = "DELETE coucou je suis heureux";
    EXPECT_EQ(test.find_method(), (size_t) 0);
    test._buffer = "coucou GET coucou je suis heureux";
    EXPECT_EQ(test.find_method(), (size_t) 7);
    test._buffer = "coucou POST coucou je suis heureux";
    EXPECT_EQ(test.find_method(), (size_t) 7);
    test._buffer = "coucou DELETE coucou je suis heureux";
    EXPECT_EQ(test.find_method(), (size_t) 7);
    test._buffer = "";
    EXPECT_EQ(test.find_method(), test._buffer.npos);
    test._buffer = "coucou je suis heureux et il n'y a pas de methode ici";
    EXPECT_EQ(test.find_method(), test._buffer.npos);
}
