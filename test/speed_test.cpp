/* #####################################################################################################################

               """          speed_test.cpp
        -\-    _|__
         |\___/  . \        Created on 25 Aug. 2024 at 11:48
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "Logger.hpp"
#include "gtest/gtest.h"
#include <cstddef>
#include <ostream>

TEST(speed_test, no_debug_log) {
    for (size_t i = 0; i < 100000; i++) {
        int a = i + 3;
        (void) a;
    }
}

TEST(speed_test, debug_log) {
    debug.disable();
    for (size_t i = 0; i < 100000; i++) {
        debug.log() << "coucou" << i << std::endl;
        int a = i + 3;
        (void) a;
    }
}
