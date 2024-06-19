/* #############################################################################

               """          GetFileStrategy.hpp
        -\-    _|__
         |\___/  . \        Created on 19 Jun. 2024 at 11:25
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_GETFILESTRATEGY_HPP_
#define INCLUDE_SRC_GETFILESTRATEGY_HPP_

#include "ResponseBuildingStrategy.hpp"
#include <string>

class GetFileStrategy : public ResponseBuildingStrategy {
    public:
        GetFileStrategy(std::string &location, ResponseBuildingStrategy &state);
        ~GetFileStrategy();

        void    buildResponse();
        bool    fill_buffer();
        void    save_mem();

    private:
        std::string _location;
};

#endif  // INCLUDE_SRC_GETFILESTRATEGY_HPP_
