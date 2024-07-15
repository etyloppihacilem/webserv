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

/**
  This object contain the body of a response. To save memory, the body may not be generated or alocated yet and will
  be constructed while it is written. This allows the server to send large files or large generated content
  (GetIndexStrategy for example) without blocking on a socket.

  This is meant to be used only in Response object, as Response will handle the proper construction depending on
  estimated size and body type.
  */
class BodyWriter {
    public:
        BodyWriter(ResponseBuildingStrategy &state);
        virtual ~BodyWriter() = 0;

        virtual std::string         generate(size_t size    = BUFFER_SIZE) = 0;
        virtual size_t              length() const          = 0;    ///< Returns length of body sent or to be sent,
                                                                    ///< depending on BodyWriter type.
        bool                        is_done() const;                ///< Returns if BodyWriter is done sending.
        virtual void                save_mem() = 0;                 ///< Save allocated heap.

    protected:
        bool                        _done;                          ///< True if body is sent.
        ResponseBuildingStrategy    *_strategy;                     ///< Strategy is needed to generate content of body
};

std::ostream &operator<<(std::ostream &os, BodyWriter &obj);        // TODO:is this useful ?? NOT IMPLEMENTED YET

#endif  // INCLUDE_SRC_BODYWRITER_HPP_
