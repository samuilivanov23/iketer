#ifndef ABSTRACTPROTOCOLHANDLER_H
#define ABSTRACTPROTOCOLHANDLER_H

#include "iprocedureinvokationhandler.h"
#include "iclientconnectionhandler.h"
#include "../common/procedure.h"
#include "string"
#include <map>

#define KEY_REQUEST_METHODNAME "method"
#define KEY_REQUEST_ID "id"
#define KEY_REQUEST_PARAMETERS "params"
#define KEY_RESPONSE_RESULT "result"
#define KEY_RESPONSE_ERROR "error"

namespace itsp 
{
    class AbstractProtocolHandler : public IProtocolHandler 
    {
        public:
            AbstractProtocolHandler( IprocedureInvokationHandler &procedureInvokationHandler );
            virtual ~AbstractProtocolHandler();

            void HandleRequest( const Json::Value &request, std::string &returnValue );
            virtual void AddProcedure( const Procedure &procedure );
            virtual void HandleJsonRequest( const Json::Value &request, Json::Value &response ) = 0;
            virtual void ValidateRequestFields( const Json::Value &value ) = 0;
            virtual void WrapResult( const Json::Value &request, Json::Value &response, Json::Value &returnValue ) = 0;
            virtual void WrapError( const Json::Value &request, int errorCode, std::string &errorMessage, Json::Value &resultError ) = 0;
            virtual procedure_t GetRequestType( const Json::Value &request );


        protected:
            IProcedureInvokationHandler &procedureInvokationHandler;
            std::map<std::string, Procedure> procedures;

            void ProcessRequest( const Json::Value &request, Json::Value response );
            int ValidateRequest( const Json::Value &request );
    };
}

#endif //ABSTRACTPROTOCOLHANDLER_H
