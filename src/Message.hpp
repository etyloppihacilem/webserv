/* #############################################################################

               """          Message.hpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 17:51
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_MESSAGE_HPP_
#define INCLUDE_SRC_MESSAGE_HPP_

#include <string>

class Message {
public:
    Message();
    Message(const Message&);
    ~Message();

    Message &operator=(const Message&);
private:
    std::string _body;
};

#endif  // INCLUDE_SRC_MESSAGE_HPP_
