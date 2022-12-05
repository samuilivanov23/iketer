#ifndef ABSTRACT_SERVER_H
#define ABSTRACT_SERVER_H

#include "abstractserverconnector.h"
#include "iclientconnectionhandler.h"
#include "iprocedureinvokationhandler.h"
#include "requesthandlerfactory.h"
#include "../common/procedure.h"

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

            //AbstractServer( AbstractServerConnector &connector, serverVersion_t type = JSONRPC_SERVER_V2 ) {};
            //virtual ~AbstractServer() {};
            //bool StartListening();
            //bool StopListening();
            //virtual void HandleMethodCall( Procedure &procedure, const Json::Value &input, Json::Value &output );
            //virtual void HandleNotificationCall( Procedure &procedure, methodPointer_t methodPointer );


			AbstractServer( AbstractServerConnector &connector, serverVersion_t type = JSONRPC_SERVER_V2 )
			{
				this->connection = connector;
				this->handler = RequestHandlerFactory::CreateProtocolHandler( type, this );
				connector.SetHandler( this->handler );
			}

			virtual ~AbstractServer() { delete this->handler; }

			bool StartListening() { return connection.StartListening(); }

			bool StopListening() { return connection.StopListening(); }

			virtual void HandleMethodCall( Procedure &procedure, const Json::Value &input, Json::Value &output )
			{
				S *instance = dynamic_cast<S *>( this );
				( instance->*methods[procedure.GetProcedureName()] )( input, output );
			}

			virtual void HandleNotificationCall( Procedure &procedure, Json::Value &input )
			{
				S *instance = dynamic_cast<S *>( this );
				( instance->*notifications[procedure.GetProcedureName()] )( input );
			}


        protected:
			bool BindAndAddMethod( const Procedure &procedure, methodPointer_t methodPointer )
			{
				if( procedure.GetProcedureType() == RPC_METHOD && !this->SymbolExists( procedure.GetProcedureName() ) )
				{
					this->handler->AddProcedure( procedure );
					this->methods[procedure.GetProcedureName()] = methodPointer;
					return  true;
				}

				return false;
			}


			bool BindAndAddNotification( const Procedure &procedure, notificationPointer_t notifPointer )
			{
				if( procedure.GetProcedureType() == RPC_NOTIFICATION && !this->SymbolExists( procedure.GetProcedureName() ) )
				{
					this->handler->AddProcedure( procedure );
					this->notifications[procedure.GetProcedureName()] = notifPointer;
					return true;
				}

				return false;
			}


        private:
            AbstractServerConnector &connection;
            IProtocolHandler *handler;
            std::map<std::string, methodPointer_t> methods;
            std::map<std::string, notificationPointer_t> notifications;

            bool SymbolExists( const std::string &name );         
    };
}

#endif //ABSTRACT_SERVER_H
