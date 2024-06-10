/* #############################################################################

               """          BodyWriter.hpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 10:26
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_BODYWRITER_HPP_
#define INCLUDE_SRC_BODYWRITER_HPP_

#include "todo.hpp"
#include <cstddef>
#include <iostream>
#include <string>

class ResponseBuildingStrategy;

/*
 * this is meant to be used only in response object
 * response will handle the right construction
 */
class BodyWriter {
    public:
        BodyWriter(ResponseBuildingStrategy *state);
        virtual ~BodyWriter() = 0;

        virtual std::string         generate(size_t size = BUFFER_SIZE) = 0;
        bool                        is_done() const;

    private:
        bool                        _done;
        ResponseBuildingStrategy    *_strategy; // interdependance
        std::string                 _buffer;
};

std::ostream &operator<<(std::ostream &os, BodyWriter &obj); // TODO is this useful

#endif  // INCLUDE_SRC_BODYWRITER_HPP_
