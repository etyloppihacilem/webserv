/* #############################################################################

               """          BodyWriter.cpp
        -\-    _|__
         |\___/  . \        Created on 10 Jun. 2024 at 10:26
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#include "BodyWriter.hpp"
#include <ostream>

BodyWriter::BodyWriter() : _done(false), _strategy(0) {}

BodyWriter::BodyWriter(ResponseBuildingStrategy &strategy) : _done(false), _strategy(&strategy) {}

BodyWriter::~BodyWriter() {}

std::ostream &operator<<(std::ostream &os, BodyWriter &obj) {
    os << obj.generate();
    return os;
}

bool BodyWriter::is_done() const {
    return _done;
}
