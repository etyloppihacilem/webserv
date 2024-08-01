/* #############################################################################

               """          CGIStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 26 Jun. 2024 at 11:41
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_CGISTRATEGY_HPP_
#define INCLUDE_SRC_CGISTRATEGY_HPP_

#include "ClientRequest.hpp"
#include "ResponseBuildingStrategy.hpp"
#include "todo.hpp"
#include <cstddef>
#include <map>
#include <string>

class CGIStrategy : public ResponseBuildingStrategy {
    public:
        CGIStrategy(const std::string &location, ClientRequest *_request);
        ~CGIStrategy();

        bool build_response();
        bool fill_buffer(std::string &buffer, size_t size = MAX_BODY_BUFFER);
        void save_mem();

    private:
        char **generate_env(const std::map<std::string, std::string> &env) const;

        std::string    _location;
        ClientRequest *_request;
};

#endif // INCLUDE_SRC_CGISTRATEGY_HPP_
