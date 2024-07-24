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

/**
  This body object is used to read the body of an incoming ClientRequest. The body can be either BodyLength or BodyChunk
  depending on the request headers.

  Using pop or clean only one time will break the body object ability to use get. This is protected and a warning will
  be issued.

  DO NOT USE BOTH TECHNIQUES AT THE SAME TIME !
 */
class Body {
    public:
        Body(int fd, std::string &buffer);
        virtual ~Body() = 0;

        virtual std::string &get()  = 0;        ///< Is used to read the body as a whole and saves it in the _body
                                                ///< string.
        virtual std::string pop()   = 0;        ///< Is used to save memory and read the body chunk by chunk.
        virtual void        clean() = 0;        ///< Used to empty _body. Do dealocate memory
        bool    is_done() const;                ///< Returns true if body was read completely
        size_t  length() const;                 ///< Returns length read by object
        void    save_mem();                     ///< Used to save memory in case of heap going missing

    protected:
        int         _fd;                        ///< File descriptor of the incoming socket
        std::string &_buffer;                   ///< Buffer of ClientRequest object
        bool        _done;                      ///< Is true if body is done being read
        bool        _uniform;                   ///< Used to protect get() usage if pop() or clean() are used
        std::string _body;                      ///< Whole body if get() is used, always empty if pop() is in use.
        size_t      _total;                     ///< Size read or to be read (in case BodyLength)
        // TODO:check max_body_length implementation
};

#endif  // INCLUDE_SRC_BODY_HPP_
