/* #############################################################################

               """          MemoryHandler.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 15:26
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "MemoryHandler.hpp"
#include "Logger.hpp"
#include <cstddef>
#include <new>
#include <ostream>

MemoryHandler::MemoryHandler(size_t size) : _storage(0), _size(size), _allocated(false) {}

MemoryHandler::~MemoryHandler() {
    if (_allocated)
        delete[] _storage;
}

bool MemoryHandler::allocate() {
    if (_allocated) {
        warn.log() << "Memory handler is allready allocated." << std::endl;
        return _allocated;
    }
    try {
        _storage = new char[_size];
        info.log() << "Memory handler succesfully allocated." << std::endl;
    } catch (std::bad_alloc &e) {
        error.log() << "FATAL could not allocate handler buffer memory." << std::endl;
        return _allocated;
    }
    _allocated = true;
    return _allocated;
}

bool MemoryHandler::deallocate() {
    if (!_allocated) {
        warn.log() << "Memory handler is not allocated." << std::endl;
        return _allocated;
    }
    delete[] _storage;
    info.log() << "Memory handler succesfully deallocated." << std::endl;
    _allocated = false;
    return _allocated;
}

bool MemoryHandler::is_allocated() const {
    return _allocated;
}

MemoryHandler mem(524288); // 500kiB
