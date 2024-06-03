/* #############################################################################

               """          BodyChunk.cpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:52
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyChunk.hpp"
#include "Body.hpp"

BodyChunk::BodyChunk(int fd, std::string &buffer):
    Body(fd, buffer) {}

BodyChunk::~BodyChunk() {}
