#include "abstractprotocolhandler.h"
#include "../common/errors.h"
#include <sstream>
#include <map>

using namespace std;
using namespace itsp;

AbstractProtocolHandler::AbstractProtocolHandler( IProcedureInvokationHandler &procedureInvokationHandler ) 
{ 
    this->procedureInvokationHandler = procedureInvokationHandler;
}

AbstractProtocolHandler::~AbstractProtocolHandler() {}

void AbstractProtocolHandler::AddProcedure( const Procedure &procedure ) { this->procedures[procedure.GetProcedureName] = procedure; }

void AbstractProtocolHandler::HandleRequest( const Json::Value &request, std::string &returnValue )
{
    Json::Value request_, response_;
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder["indentation"] = "";

    try
    {
        istringstream( request ) >> request_;
        this->HandleJsonRequest( request_, response_ );
    }
    catch( const Json::Exception &exception )
    {
        this->WrapError( request, Errors::ERROR_RPC_JSON_PARSE_ERROR, Errors::GetErrorMessage( Errors::ERROR_RPC_JSON_PARSE_ERROR ), response );
    }

    if( response_ != Json::nullValue ) 
    { 
        returnValue = Json::writeString( writerBuilder, response_ ); 
    }
}

void AbstractProtocolHandler::ProcessRequest( const Json::Value &request, Json::Value &response )
{
    Procedure &method = this->procedures[request[KEY_REQUEST_METHODNAME].asString()];
    Json::Value resultResponse;

    if( method.GetProcedureType() == RPC_METHOD )
    {
        procedureInvokationHandler.HandleMethodCall( method, request[KEY_REQUEST_PARAMETERS], resultResponse );
        this->WrapResult( request, response, resultResponse );
    }
    else
    {
        procedureInvokationHandler.HandleNotificationCall( method, request[KEY_REQUEST_PARAMETERS] );
        response = Json::nullValue;
    }
}

int AbstractProtocolHandler::ValidateRequest( const Json::Value &request )
{
    int error = 0;
    Procedure procedure;
    if( !this->ValidateRequestFields( request ) )
    {
        error = Errors:ERROR_RPC_INVALID_REQUEST;
    }
    else
    {
        map<std::string, Procedure>::iterator i = this->procedures.find( request[KEY_REQUEST_METHODNAME].asString() );
        if( i != this->procedures.end() )
        {
            procedure = i->second;
            if( this->GetRequestType( request ) == RPC_METHOD && procedure.GetProcedureType() == RPC_NOTIFICATION )
            {
                error = Errors::ERROR_SERVER_PROCEDURE_IS_NOTIFICATION;
            }
            else if( this->GetRequestType( request ) == RPC_NOTIFICATION && procedure.GetProcedureType() == RPC_METHOD )
            {
                error = Errors::ERROR_SERVER_PROCEDURE_IS_METHOD;
            }
            else if( !procedure.ValidateParameters( request[KEY_REQUEST_PARAMETERS] ) )
            {
                error = Errors::ERROR_RPC_INVALID_PARAMS;
            }
        }
        else
        {
            error = Errors::ERROR_RPC_METHOD_NOT_FOUND;
        }
    }

    return error;
}
