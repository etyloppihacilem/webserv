#include "Logger.hpp"
#include "gtest/gtest.h"

int main(int argc, char **argv) {
    // Logger::force();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
