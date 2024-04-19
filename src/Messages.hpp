/* #############################################################################

               """          Messages.hpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 17:51
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_MESSAGES_HPP_
#define INCLUDE_SRC_MESSAGES_HPP_

#include <string>

class Messages {
public:
    Messages();
    Messages(const Messages&);
    ~Messages();

    Messages &operator=(const Messages&);
private:
    std::string _body;
};

#endif  // INCLUDE_SRC_MESSAGES_HPP_
