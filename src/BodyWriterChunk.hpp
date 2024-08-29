/* #############################################################################

               """          BodyWriterChunk.hpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 10:52
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_BODYWRITERCHUNK_HPP_
#define INCLUDE_SRC_BODYWRITERCHUNK_HPP_

#include "BodyWriter.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <cstddef>
#include <string>

class BodyWriterChunk : public BodyWriter {
    public:
        BodyWriterChunk(ResponseBuildingStrategy &strategy);
        ~BodyWriterChunk();

        std::string generate(size_t size = BUFFER_SIZE);
        size_t      length() const;
        void        save_mem();

    private:
        size_t _length;
};

#endif // INCLUDE_SRC_BODYWRITERCHUNK_HPP_
