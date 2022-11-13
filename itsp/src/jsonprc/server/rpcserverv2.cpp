#include "../common/errors.h"
#include "rpcserverv2.h"
#include <iostream>

using namespace std;
using namespace itsp;

RpcServerV2::RpcServerV2( IProcedureInvokationHandler &procedureInvokationHandler ) : AbstractProtocolHandler( procedureInvokationHandler ) {}

void RpcServerV2::HandleJsonRequest( const Json::Value &request, Json::Value response )
{
    if( request.isArray() )
    {
        this->HandleBatchRequests( request, response );
    }
    else if( request.isObject() )
    {
        this->HandleSingleRequest( request, response );
    }
    else 
    {
        this->WrapError( Json::nullValue, Errors::ERROR_RPC_INVALID_REQUEST, Errors::GetErrorMessage( Errors::ERROR_RPC_INVALID_REQUEST ), response );
    }
}

void RpcServerV2::HandleSingleRequest( const Json::Value &request, Json::Value &response )
{
    int error = this->ValidateRequest( request );
    if( error == 0 )
    {
        try
        {
            this->ProcessRequest( request, response );
        }
        catch( const JsonRpcException &exception )
        {
            this->WrapException( request, exception, response );
        }
    }
    else
    {
        this->WrapError( request, error, Errors::GetErrorMessage( error ), response );
    }
}

void RpcServerV2::HandleBatchRequests( const Json::Value requests, Json::Value &response )
{
    if( requests.isEmpty() )
    {
        this->WrapError( Json::nullValue, Errors::ERROR_RPC_INVALID_REQUEST, Errors:GetErrorMessage( Errors::ERROR_RPC_INVALID_REQUEST ), response );
    }
    else
    {
        for( uint16_t i = 0; i < request.size(); i++ )
        {
            Json::Value currentRequestResult;
            this->ProcessRequest( requests[i], currentRequestResult );
            if( currentRequestResult != Json::nullValue ) 
            { 
                response.append( currentRequestResult );
            }
        }
    }
}

bool RpcServerV2::ValidateRequestFields( const Json::Value &value )
{
    if( !request.isObject() ) { return false; }
    
    if( !( request.isMember( KEY_REQUEST_METHODNAME ) && request[KEY_REQUEST_METHODNAME].isString() ) ) { return false; }
    
    if( !( request.isMember( KEY_REQUEST_VERSION ) && request[KEY_REQUEST_VERSION].isString() && request[KEY_REQUEST_VERSION]==JSON_RPC_VERSION2 ) ) { return false; }
    if( request.isMember( KEY_REQUEST_ID ) && 
        !( request[KEY_REQUEST_ID].isInt() || request[KEY_REQUEST_ID].isString() || request[KEY_REQUEST_ID].isNull() ) ) { return false; }
    
    if( request.isMember( KEY_REQUEST_PARAMETERS ) &&
        !( request[KEY_REQUEST_PARAMETERS].isObject() || request[KEY_REQUEST_PARAMETERS].isArray() || request[KEY_REQUEST_PARAMETERS].isNull ) ) { return false; }

    return true;
}

void RpcServerV2::WrapResult( const Json::Value &request, Json::Value &response, Json::Value &returnValue )
{
    response[KEY_REQUEST_VERSION] = JSON_RPC_VERSION2;
    response[KEY_RESPONSE_RESULT] = returnValue;
    response[KEY_REQUEST_ID] = request[KEY_REQUEST_ID];
}

void RpcServerV2::WrapError( const Json::Value request, int errorCode, const std::string errorMessage, Json::Value &resultError )
{
    resultError["jsonrpc"] = "2.0";
    resultError["error"]["code"] = errorCode;
    resultError["error"]["message"] = errorMessage;

    if( request.isObject() && request.isMember( "id" ) && ( request["id"].isNull() || request["id"].isInt() || request["id"].isString() ) )
    {
        resultError["id"] = request["id"];
    }
    else
    {
        resultError["id"] = Json::nullValue;
    }
}

void RpcServerV2::WrapException( const Json::Value &request, const JsonRpcException &exception, Json::Value &returnException )
{
    this->WrapError( request, exception.GetCode(), exception.GetMessage(), returnException );
    returnException["error"]["data"] = exception.GetData();
}

procedure_t RpcServerV2::GetRequestType( const Json::Value &request )

{
    if( request.isMember( KEY_REQUEST_ID ) ) { return RPC_METHOD; }

    return RPC_NOTIFICATION;
}