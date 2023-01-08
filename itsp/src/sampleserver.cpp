#include <iostream>
#include "jsonprc/server.h"
#include "jsonprc/server/connectors/httpserverconnector.h"
#include <stdio.h>
#include <string>

using namespace itsp;
using namespace std;

class SampleServer : public AbstractServer<SampleServer> 
{
	public:
		SampleServer( HttpServer &server ) : AbstractServer<SampleServer>( server ) 
		{
			this->BindAndAddMethod( Procedure ("sayHello", PARAMS_BY_NAME, JSON_STRING, "name", JSON_STRING, NULL ), &SampleServer::sayHello );
			this->BindAndAddNotification( Procedure( "notifyServer", PARAMS_BY_NAME, NULL ), &SampleServer::notifyServer );
		}

		// method
		void sayHello( const Json::Value &request, Json::Value &response ) { response = "Hello: " + request["name"].asString(); }

		// notification
		void notifyServer( const Json::Value &request ) 
		{
			( void )request;
			cout << "server received some Notification" << endl;
		}
};

int main() 
{
	try 
	{
		HttpServer server( 8383 );
		SampleServer serv( server );
		if ( serv.StartListening() ) 
		{
	  		cout << "Server started successfully" << endl;
	  		getchar();
	  		serv.StopListening();
		}
	   	else 
		{
			cout << "Error starting Server" << endl;
		}
	}
	catch ( itsp::JsonRpcException &e ) 
	{
		cerr << e.what() << endl;
	}
	// curl --data
	// "{\"jsonrpc\":\"2.0\",\"method\":\"sayHello\",\"id\":1,\"params\":{\"name\":\"peter\"}}"
	// localhost:8383  curl --data
	// "{\"jsonrpc\":\"2.0\",\"method\":\"notifyServer\", \"params\": null}"
	// localhost:8383
}
