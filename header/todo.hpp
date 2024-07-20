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
# define BUFFER_SIZE 13         // arbitrary for testing
#else
# define BUFFER_SIZE 1024       // 1 KiB arbitrary too but bigger
#endif

#define SERVER_SOFTWARE "webserve/0.1"

#define MAX_BODY_BUFFER 1048576 // 1MiB

#endif  // INCLUDE_HEADER_TODO_HPP_
