/* #############################################################################

               """          BodyWriterLength.hpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 10:54
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_BODYWRITERLENGTH_HPP_
#define INCLUDE_SRC_BODYWRITERLENGTH_HPP_

#include "BodyWriter.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <cstddef>
#include <string>

class BodyWriterLength: public BodyWriter {
    public:
        BodyWriterLength(ResponseBuildingStrategy &state);
        ~BodyWriterLength();

        std::string generate(size_t size = BUFFER_SIZE);
    private:
};

#endif  // INCLUDE_SRC_BODYWRITERLENGTH_HPP_
