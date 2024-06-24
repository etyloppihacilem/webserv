/* #############################################################################

               """          StringUtils.cpp
        -\-    _|__
         |\___/  . \        Created on 12 Jun. 2024 at 14:48
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "StringUtils.hpp"
#include <cstddef>
#include <string>

/**
  Shrink std::string allocation to match size.
  Saves memory.
  */
void shrink_to_fit(std::string &str) {
    std::string(str.data(), str.size()).swap(str);
}

/**
  Extract extension from path.
  */
std::string extract_extension(std::string &s) {
    size_t  slash       = s.find_last_of("/");
    size_t  extension   = s.find_last_of(".");

    if (slash == s.npos)
        slash = 0;
    if (extension == s.npos || (slash == 0 && extension == slash) || (slash != 0 && extension == slash + 1))
        return "";
    return s.substr(extension + 1, s.length() - (extension + 1));
}
