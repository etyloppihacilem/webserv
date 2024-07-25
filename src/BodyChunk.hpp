/* #############################################################################

               """          BodyChunk.hpp
        -\-    _|__
         |\___/  . \        Created on 03 Jun. 2024 at 14:52
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_BODYCHUNK_HPP_
#define INCLUDE_SRC_BODYCHUNK_HPP_

#include "Body.hpp"
#include <cstddef>
#include <string>

#ifdef TESTING
#include "gtest/gtest.h"
#endif

/**
  BodyChunk is meant to read chunked body from ClientRequest.

  A chunk body will have the following structure :
  ```
  SIZE\r\n                  // a chunk
  <content of SIZE>\r\n
  ...                       // some more chunks
  0\r\n                     // end chunk
  <Discarded trailing optional stuff>\r\n
  ```
  with SIZE being the hex number of bytes.
  */
class BodyChunk: public Body {
    public:
        BodyChunk(int fd, std::string &buffer);
        ~BodyChunk();

        std::string &get();
        std::string pop();
        void        clean();

    private:
        size_t  _bytes_remaining;   ///< Number of bytes left to read in current chunk
        bool    _eoc;               ///< tells if end of chunk to remove \r\n
        bool    _trailing;          ///< Tells if body is read but trailing info is left to read and discard

        size_t  read_body();
        void    init_chunk();
        bool    is_hex(int c);
#ifdef TESTING
        FRIEND_TEST(BodyChunkTestSuite, is_hex);
        FRIEND_TEST(BodyChunkTestSuite, init_chunk);
        FRIEND_TEST(BodyChunkTestSuite, init_chunk_end);
        FRIEND_TEST(BodyChunkTestSuite, init_chunk_bad);
        FRIEND_TEST(BodyChunkTestSuite, init_chunk_none);
        FRIEND_TEST(BodyChunkTestSuite, read_body);
        FRIEND_TEST(BodyChunkTestSuite, get);
#endif
};

#endif  // INCLUDE_SRC_BODYCHUNK_HPP_
