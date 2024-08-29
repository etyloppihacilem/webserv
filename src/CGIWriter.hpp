/* #####################################################################################################################

               """          CGIWriter.hpp
        -\-    _|__
         |\___/  . \        Created on 29 Aug. 2024 at 13:28
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

##################################################################################################################### */

#ifndef INCLUDE_SRC_CGIWRITER_HPP_
#define INCLUDE_SRC_CGIWRITER_HPP_

#include "BodyWriter.hpp"
#include "HttpStatusCodes.hpp"
#include "Response.hpp"
#include "todo.hpp"
#include <cstddef>
#include <string>

class CGIWriter : public BodyWriter {
    public:
        CGIWriter(ResponseBuildingStrategy &strategy);
        ~CGIWriter();

        std::string generate(size_t size = BUFFER_SIZE);
        size_t      length() const;
        void        save_mem();

    private:
        void        send_error(HttpCode code = InternalServerError);
        void        init();
        Response   &_response;
        std::string _buffer;
        size_t      _total;
        bool        _init; ///< Wether init needs to be done
        bool        _cgi_done;
};

#endif // INCLUDE_SRC_CGIWRITER_HPP_
