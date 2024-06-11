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
#include <string>

class BodyWriterChunk : public BodyWriter {
    public:
        BodyWriterChunk(ResponseBuildingStrategy &state);
        ~BodyWriterChunk();

        std::string generate(size_t size = BUFFER_SIZE);
    private:
};

#endif  // INCLUDE_SRC_BODYWRITERCHUNK_HPP_
