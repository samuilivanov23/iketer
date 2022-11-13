#ifndef RPCSERVERV2_H
#define RPCSERVERV2_H

#include <string>
#include <vector>
#include <map>

#include "abstractprotocolhandler.h"
#include "../common/exception.h"

#define KEY_REQUEST_VERSION "jsonrpc"
#define JSON_RPC_VERSION "2.0"

namespace itsp
{
    class RpcServerV2 : public AbstractProtocolHandler 
    {
        public:
            RpcServerV2( IProcedureInvokationHandler &procedureInvokationHandler );

            void HandleJsonRequest( const Json::Value &request, Json::Value &response );
            bool ValidateRequestFields( const Json::Value &value );
            void WrapResult( const Json::Value &request, Json::Valie &response, Json::Value &returnValue );
            void WrapError( const Json::Value &request, int errorCode, const std::string &errorMessage, Json::Value &resultError );
            void WrapException( const Json::Value &request, const JsonRpcException &exception, Json::Value &returnException  );
            procedure_t GetProcedureType( const Json::Value &request );

        private:
            void HandleSingleRequest( const Json::Value &request, Json::Value &response );
            void HandleBatchRequests( const Json::Value &requests, Json::Value $response );
    };
}

#endif //RPCSERVERV2_H
