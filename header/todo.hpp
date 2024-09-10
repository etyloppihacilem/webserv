/* #############################################################################

               """          todo.hpp
        -\-    _|__
         |\___/  . \        Created on 04 Jun. 2024 at 11:33
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_HEADER_TODO_HPP_
#define INCLUDE_HEADER_TODO_HPP_

#ifdef TESTING
# define BUFFER_SIZE 13 // arbitrary for testing
#else
# define BUFFER_SIZE 4096 // 1 KiB arbitrary too but bigger
#endif

#define PIPE_BUFFER_SIZE BUFFER_SIZE // bc 1KiB is already pipe max

#define TIME_LENGTH 32 // 29 normally
// Sun, 06 Nov 1994 08:49:37 GMT

#define SERVER_SOFTWARE "jeon/0.1"

#define MAX_BODY_BUFFER 1048576 // 1MiB

#endif // INCLUDE_HEADER_TODO_HPP_
