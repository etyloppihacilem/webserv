/* #############################################################################

               """          StringUtils.hpp
        -\-    _|__
         |\___/  . \        Created on 12 Jun. 2024 at 14:48
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_STRINGUTILS_HPP_
#define INCLUDE_SRC_STRINGUTILS_HPP_

#include <string>

std::string add_trailing_slash(const std::string &str);
std::string clean_trailing_slash(const std::string &str);
// redefinition because is not implemented in c++98.
void        shrink_to_fit(std::string &s);
std::string extract_extension(const std::string &s);
std::string extract_basename(const std::string &s);

#endif // INCLUDE_SRC_STRINGUTILS_HPP_
