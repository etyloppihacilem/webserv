/* #############################################################################

               """          MemoryHandler.hpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 15:26
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_MEMORYHANDLER_HPP_
#define INCLUDE_SRC_MEMORYHANDLER_HPP_

#include <cstddef>

class MemoryHandler {
    public:
        MemoryHandler(size_t size = 1024);
        ~MemoryHandler();

        bool    allocate();
        bool    deallocate();

    private:
        char    *_storage;
        size_t  _size;
        bool    _allocated;
};

extern MemoryHandler mem;

#endif  // INCLUDE_SRC_MEMORYHANDLER_HPP_
