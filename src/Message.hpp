/* #############################################################################

               """          Message.hpp
        -\-    _|__
         |\___/  . \        Created on 19 Apr. 2024 at 17:51
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_MESSAGE_HPP_
#define INCLUDE_SRC_MESSAGE_HPP_

#include "gtest/gtest_prod.h"
#include <map>
#include <sstream>
#include <string>
#include "HttpMethods.hpp"
#include "HttpStatusCodes.hpp"
#ifdef TESTING
#include "gtest/gtest.h"
#endif

class Message {
    public:
        Message();
        Message(const Message&);
        ~Message();

        Message                            &operator=(const Message&);
        void                               parse(const std::string &in);
    private:
        HttpMethod                         parse_method(const std::string &method);
        std::string                        _method;
        std::string                        _target;
        std::map<std::string, std::string> _header;
        std::string                        _body;
        HttpCode                           _status;

#ifdef TESTING
        FRIEND_TEST(MessageTest, ParseMethodTest);
#endif
};

#endif  // INCLUDE_SRC_MESSAGE_HPP_
