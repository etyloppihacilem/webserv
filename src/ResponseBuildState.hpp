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
template < class ServerClass = Server, class RouteClass = Route > // template is there for testing purposes
class ResponseBuildState : public ProcessState {
    public:
        ResponseBuildState(int socket, ClientRequest *request, int port);
        ResponseBuildState(
            int          socket,
            HttpCode     code,
            ServerClass *server = 0
        ); // equivalent of calling recovery on ErrorBuildingStrategy
        ~ResponseBuildState();

        t_state process();

        ResponseBuildingStrategy *get_response_strategy();
        HttpCode                  get_status() const;

        void save_mem();

    private:
        void init_strategy();
        void init_strategy(HttpCode code);

        ClientRequest            *_request;
        const ServerClass        *_server;
        ResponseBuildingStrategy *_strategy;
        bool                      _recovery;
        HttpCode                  _code;
        friend class ResponseBuildStateFixture;
};

#endif // INCLUDE_SRC_RESPONSEBUILDSTATE_HPP_
