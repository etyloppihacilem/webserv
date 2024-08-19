/* #############################################################################

               """          ResponseBuildState.hpp
        -\-    _|__
         |\___/  . \        Created on 06 Jun. 2024 at 13:11
         \     /(((/        by hmelica
          \___/)))/         hmelica@student.42.fr

############################################################################# */

#ifndef INCLUDE_SRC_RESPONSEBUILDSTATE_HPP_
#define INCLUDE_SRC_RESPONSEBUILDSTATE_HPP_

#include "BodyWriter.hpp"
#include "ClientRequest.hpp"
#include "HttpStatusCodes.hpp"
#include "ProcessState.hpp"
#include "Route.hpp"
#include "Server.hpp"

/**
  This class is in charge of initiating the right strategy for the given ClientRequest and generating the response.
  It will be done when the response will be built.
  */
template <class ServerClass = Server, class RouteClass = Route> // template is there for testing purposes
class ResponseBuildState : public ProcessState {
    public:
        ResponseBuildState(int socket, ClientRequest *request, ServerClass &server);
        ResponseBuildState(int socket, HttpCode code); // equivalent of calling recovery on ErrorBuildingStrategy
        ~ResponseBuildState();

        t_state                   process();
        ClientRequest            *get_request();
        ResponseBuildingStrategy *get_response_strategy();
        void                      save_mem();

    private:
        void init_strategy();
        void init_strategy(HttpCode code);

        ClientRequest            *_request;
        ServerClass               _server;
        ResponseBuildingStrategy *_strategy;
        bool                      _recovery;
        HttpCode                  _code;
};

#endif // INCLUDE_SRC_RESPONSEBUILDSTATE_HPP_
