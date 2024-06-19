/* #############################################################################

               """          ResponseBuildingStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:39
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_RESPONDEBUILDINGSTRATEGY_CPP_
#define INCLUDE_SRC_RESPONDEBUILDINGSTRATEGY_CPP_

#include "Response.hpp"
#include "ResponseBuildState.hpp"
#include "todo.hpp"
#include <cstddef>
#include <string>

/**
  This is the abstract class to create object in charge of handling ClientRequest and generating an appropriate Response.
  */
class ResponseBuildingStrategy {
    public:
        ResponseBuildingStrategy(ResponseBuildState &state);
        ~ResponseBuildingStrategy();

        virtual void        buildResponse() = 0;
        Response            &get_response();
        bool                is_done() const;
        virtual bool        fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER) = 0;
        size_t              get_estimated_size() const;
        virtual void        save_mem();

    protected:
        ResponseBuildState  *_state;            ///< ResponseBuildState to have interdependence and access ClientRequest
        Response            _response;          ///< The response object
        bool                _done;              ///< State of the current object
        size_t              _estimated_size;    ///< Estimated size of response body if needed
        std::string         _buffer;            ///< String buffer in case too much is read. Not used yet ig TODO check
        ///< if used
};

#endif  // INCLUDE_SRC_RESPONDEBUILDINGSTRATEGY_CPP_

// TODO optimization
// str.shrink_to_fit();
// this method free memory from previously allocated string stuff that is no longer used
