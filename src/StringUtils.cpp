/* #############################################################################

               """          StringUtils.cpp
        -\-    _|__
         |\___/  . \        Created on 12 Jun. 2024 at 14:48
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "StringUtils.hpp"
#include <string>

void shrink_to_fit(std::string &str) {
    std::string(str.data(), str.size()).swap(str);
}
