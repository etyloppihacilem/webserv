/* #############################################################################

               """          Body.hpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:47
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_BODY_HPP_
#define INCLUDE_SRC_BODY_HPP_

#include <cstddef>
#include <string>

/*
 * Using pop or clean only one time will make get innefiscient
 * DO NOT USE BOTH AT THE SAME TIME
 * */
class Body {
    public:
        Body(int fd, std::string &buffer);
        virtual ~Body() = 0;

        virtual std::string &get()  = 0;
        virtual std::string pop()   = 0;
        virtual void        clean() = 0;
        bool                is_done() const;         // return true if body was read
        size_t              length() const;
        void                save_mem();

    protected:
        int                 _fd;
        std::string         &_buffer;
        bool                _done;
        bool                _uniform;   // if false, cant use get
        std::string         _body;
        size_t              _total;     // TODO implementer max_body_length
};

#endif  // INCLUDE_SRC_BODY_HPP_
