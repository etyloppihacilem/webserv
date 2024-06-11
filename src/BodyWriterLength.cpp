/* #############################################################################

               """          BodyWriterLength.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 10:54
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyWriterLength.hpp"
#include "BodyWriter.hpp"
#include "Logger.hpp"
#include <cstddef>
#include <stdint.h>
#include <string>

BodyWriterLength::BodyWriterLength(ResponseBuildingStrategy &state):
    BodyWriter(state) {}

BodyWriterLength::~BodyWriterLength() {}

std::string BodyWriterLength::generate(size_t size) {
    if (_done)
        return ("");

    std::string temp;
    uint16_t    i = 0;

    (void) size; // size is not needed for BodyWriterLength. The point is to get the whole body in one call
    while (!(_done = _strategy->fill_buffer(temp)) && ++i) // size is MAX_BODY_BUFFER so i is to prevent infinite loop
        ;
    if (i == 0)
        error.log("BodyWriterLength : maximum fill_buffer iterations exceeded. This happens to prevent infinite loop. "
                  "Result may be incomplete. Check value of MAX_BODY_BUFFER.");
    return (temp);
}
