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
#include "todo.hpp"
#include <cstddef>
#include <string>

/**
  This is the abstract class to create object in charge of handling ClientRequest and generating an appropriate
  Response.

  All ResponseBuildingStrategy objects should run build_response() at least once (without read event, no read operation
  will be performed on first build_response() call)

  This allows a call to check _built value and go to next State immediatly in case no reading is needed.

  Response object is not detachable from ResponseBuildingStrategy as most of strategy generating body needs to access
  the fill_buffer() function from its Strategy. That is why a ResponseSendState should be constructed using a
  ResponeBuildingStrategy and not a response object.
  */
class ResponseBuildingStrategy {
    public:
        ResponseBuildingStrategy();
        virtual ~ResponseBuildingStrategy() = 0;

        virtual bool build_response() = 0;
        Response    &get_response();
        bool         is_done() const;
        bool         is_built() const;
        /**
          fill_buffer() is meant to be use by a BodyWriter object to build or read body.

          This is meant to save memory by not having the whole body in memory all at once.
          */
        virtual bool fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER) = 0;
        size_t       get_estimated_size() const;
        virtual void save_mem();

    protected:
        Response _response;       ///< The response object
        bool     _built;          ///< If response is built yet
        bool     _done;           ///< State of the current object
        size_t   _estimated_size; ///< Estimated size of response body if needed
};

#endif // INCLUDE_SRC_RESPONDEBUILDINGSTRATEGY_CPP_
