#include "requesthandlerfactory.h"
#include "rpcserverv2"

using namespace itsp;

IProtocolHandler *RequestHandlerFactory::CreateProtocolHandler( serverVersion_t serverVersionType, IprocedureInvokationHandler &procedureInvokationHandler )
{
    IProtocolHandler *resultProtocolHandler = NULL;

    switch( type )
    {
        case JSONRPC_SERVER_V2: resultProtocolHandler = new RpcServerV2( procedureInvokationHandler ); break;
    }

    return resultProtocolHandler;
}
