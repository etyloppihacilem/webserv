/* #####################################################################################################################

               """          main_test.cpp
        -\-    _|__
         |\___/  . \        Created on 02 Aug. 2024 at 14:08
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#include "Logger.hpp"
#include "global_test.h"
#include "gtest/gtest.h"
#include <filesystem>
#include <ostream>
#include <string>

bool filesystem_tests = true; // wether test using real files are run or not.

int main(int argc, char **argv) {
    Logger::force();
    // debug.disable();
#ifndef WORKDIR
# define WORKDIR ""
    filesystem_tests = false;
#endif
    if (filesystem_tests) {
        std::string cwd = std::filesystem::current_path().string();
        if (cwd != WORKDIR)
            filesystem_tests = false;
    }
    ::testing::InitGoogleTest(&argc, argv);
    auto ret = RUN_ALL_TESTS();
    if (!filesystem_tests) {
        if (std::string(WORKDIR) == "")
            warn.log() << "WORKDIR macro is not defined." << std::endl;
        else
            warn.log() << argv[0] << " is not running from " << WORKDIR << "." << std::endl;
        warn.log() << "Tests using files are therefor disabled." << std::endl;
    }
    return ret;
}
