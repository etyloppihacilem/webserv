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

/**
  MemoryHandler is an object meant to allow the code to run memory recovery procedures in case of running out of heap,
  by freeing previously saved heap.

  It will deallocate() heap previously allocate() 'd.

  As its use is quite rare, every action is logged with the info Logger.
  */
class MemoryHandler {
    public:
        MemoryHandler(size_t size = 1024 /** Size of heap to allocate */);
        ~MemoryHandler();

        bool    allocate();
        bool    deallocate();

    private:
        char    *_storage;  ///< Place where the storage happends
        size_t  _size;      ///< Size of storage
        bool    _allocated; ///< Tells if memory is allocated
};

extern MemoryHandler mem;

#endif  // INCLUDE_SRC_MEMORYHANDLER_HPP_
