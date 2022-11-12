#ifndef REQUESTHANDLERFACTORY_H
#define REQUESTHANDLERFACTORY_H

#include "iclientconnectionhandler.h"
#include "iprocedureinvokationhandler.h"

namespace itsp
{
    typedef enum { JSONRPC_SERVER_V1, JSONRPC_SERVER_V2, JSONRPC_SERVER_V1V2 } serverVersion_t;

    class RequestHandlerFactory
    {
        public:
            static IProtocolHandler *CreateProtocolHandler( serverVersion_t serverVersionType, IProcedureInvokationHandler &procedureInvokationHandler );
    };
}
