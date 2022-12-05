#include "requesthandlerfactory.h"
#include "rpcserverv2.h"

using namespace itsp;

IProtocolHandler *RequestHandlerFactory::CreateProtocolHandler( serverVersion_t serverVersionType, IProcedureInvokationHandler &procedureInvokationHandler )
{
    IProtocolHandler *resultProtocolHandler = NULL;

    switch( serverVersionType )
    {
        case JSONRPC_SERVER_V2: resultProtocolHandler = new RpcServerV2( procedureInvokationHandler ); break;
    }

    return resultProtocolHandler;
}
