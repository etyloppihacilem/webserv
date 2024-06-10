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
#include <string>

class BodyWriterLength: public BodyWriter {
    public:
        BodyWriterLength();
        ~BodyWriterLength();

        std::string generate();
    private:
};

#endif  // INCLUDE_SRC_BODYWRITERLENGTH_HPP_
