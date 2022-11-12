#ifndef ABSTRACT_SERVER_H
#define ABSTRACT_SERVER_H

#include "abstractserverconnector.h"
#include "iclientconnectionhandler.h"
#include "iprocedureinvokationhandler.h"
#include "requesthandlerfactory.h"

#include <jsonrpc/common/procedure.h>
#include <map>
#include <string>
#include <vector>

namespace itsp
{
    template <class S> class AbstractServer : public IProcedureInvokationHandler
    {
        public:
            typedef void ( S::*methodPointer_t )( const Json::Value &parameter, Json::Value &result );
            typedef void ( S::*notificationPointer_t )( const Json::Value &parameter );

            AbstractServer( AbstractServerConnector &connector, serverVersion_t type = JSONRPC_SRVER_V1 ) : connection( connector );
            virtual ~AbstractServer();
            bool StartListening();
            bool StopListening();
            virtual void HandleMethodCall( Procedure &procedure, const Json::Value &input, Json::Value &output );
            virtual void HandleNotificationCall( Procedure &procedure, methodPointer_t methodPointer );

        protected:
            bool BindAndAddMethod( const Procedure &procedure, methodPointer_t pointer );
            bool BindAndAddNotification( const Procedure &procedure, notificationPointer_t notifPointer );

        private:
            AbstractServerConnector &connection;
            IProcedureHandler *handler;
            std::map<std::string, methodPointer_t> methods;
            std::map<str::string, notificationPointer_t> notifications;

            bool SymbolExists( const std::string &name );            
    };

    template <class S> AbstractServer<S>::AbstractServer( AbstractServerConnector &connector, serverVersion_t type = JSONRPC_SERVER_V1 )
    {
        this->connection = connector;
        this->handler = RequestHandlerFactory::CreateProtocolHandler( type, this );
        connector.SetHandler( this->handler );
    }

    template <class S> ~AbstractServer<S>::AbstractServer { delete this->handler; }

    template <class S> bool AbstractServer<S>::StartListening() { return connection.StartListening(); }

    template <class S> bool AbstractServer<S>::StopListening() { return connection.StopListening(); }

    template <class S> virtual void AbstractServer<S>::HandleMethodCall( Procedure &procedure, const Json::Value &input, Json::Value &output )
    {
        S *instance = dynamic_cast<S *>( this );
        ( instance->*methods[procedure.GetProcedureName()] )( input, output );
    }

    template <class S> virtual void AbstractServer<S>::HandleNotificationCall( Procedure &procedure, Json::Value &input )
    {
        S *instance = dynamic_cast<S *>( this );
        ( instance->*notifications[procedure.GetProcedureName()] )( input );
    }

    template <class S> bool BindAndAddMethod( const Procedure &procedure, methodPointer_t methodPointer )
    {
        if( procedure.GetProcedureType() == RPC_METHOD && !this->SymbolExists( procedure.GetProcedureName() ) )
        {
            this->handler->AddProcedure( procedure );
            this->methods[procedure.GetProcedureName()] = methodPointer;
            return  true;
        }

        return false;
    }

    template <class S> bool BindAndAddNotification( const Procedure &procedure, notificationPointer_t notifPointer )
    {
        if( procedure.GetProcedureType == RPC__NOTIFICATION && !this->SymbolExists( procedure.GetProcedureName() ) )
        {
            this->handler->AddProcedure( procedure );
            this->notifications[procedure.GetProcedureName()] = notifPointer;
            return true;
        }

        return false;
    }
}

#endif //ABSTRACT_SERVER_H
