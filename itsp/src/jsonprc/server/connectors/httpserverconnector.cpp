#include "httpserverconnector.h"
#include "../../common/specificationparser.h"
#include <sstream>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;
using namespace itsp;

#define BUFFERSIZE 65536

struct mhd_ConnectionInfo 
{
    struct MHD_PostProcessor *postProcessor;    
    MHD_Connection *connection;
    stringstream request;
    HttpServer *httpServer;
    int code;
};

HttpServer::HttpServer( uint16_t port, const std::string &sslCert, const std::string &sslKey, uint32_t threads ) 
{
    this->port = port;
    this->sslCert = sslCert;
    this->sslKey = sslKey;
    this->threads = threads;
    this->isBindLocalhost = false;
	this->isRunning = false;
	this->mhdDaemon = NULL;
}

HttpServer::~HttpServer() {}

IClientConnectionHandler *HttpServer::GetConnectionHandler( const std::string &url )
{
	if( AbstractServerConnector::GetHandler() != NULL )
	{
		return AbstractServerConnector::GetHandler();
	}
	
	map<string, IClientConnectionHandler*>::iterator i = this->urlHandler.find( url );
	
	if( i != this->urlHandler.end() )
	{
		return i->second;
	}

	return NULL;
}

HttpServer &HttpServer::BindToLocalhost()
{
    this->isBindLocalhost = true;
    return *this;
}

bool HttpServer::StartListening()
{
    if( !this->isRunning )
    {
        const bool hasEpoll = ( MHD_is_feature_supported( MHD_FEATURE_EPOLL ) == MHD_YES );
        const bool hasPoll = ( MHD_is_feature_supported( MHD_FEATURE_POLL ) == MHD_YES );
        unsigned int mhdFlags = MHD_USE_DUAL_STACK;

        if( hasEpoll )
        {
            // In MHD version 0.9.44 the flag is renamed to
            // MHD_USE_EPOLL_INTERNALLY_LINUX_ONLY. 
            // In later versions both are deprecated.
            #if defined(MHD_USE_EPOLL_INTERNALLY)
                mhdFlags = MHD_USE_EPOLL_INTERNALLY;
            #else
                mhdFlags = MHD_USE_EPOLL_INTERNALLY_LINUX_ONLY;
            #endif
        }
        else if( hasPoll )
        {
            mhdFlags = MHD_USE_POLL_INTERNALLY;
        }

        
        if( this->isBindLocalhost )
        {
            memset( &this->loopbackAddress, 0, sizeof( this->loopbackAddress ) );
            loopbackAddress.sin_family = AF_INET;
            loopbackAddress.sin_port = htons( this->port );
            loopbackAddress.sin_addr.s_addr = htonl( INADDR_LOOPBACK );

            this->mhdDaemon = MHD_start_daemon( mhdFlags, 
                                                this->port, 
                                                NULL, 
                                                NULL, 
                                                HttpServer::Callback, 
                                                this, 
                                                MHD_OPTION_THREAD_POOL_SIZE, 
                                                this->threads, 
                                                MHD_OPTION_SOCK_ADDR, 
                                                ( struct sockaddr* )( &( this->loopbackAddress ) ), 
                                                MHD_OPTION_END );

        }
        else if( !this->pathSSLCert.empty() && !this->pathSSLKey.empty() )
        {
            try
            {
                SpecificationParser::GetFileContent( this->pathSSLCert, this->sslCert );
                SpecificationParser::GetFileContent( this->pathSSLKey, this->sslKey );

                this->mhdDaemon = MHD_start_daemon( MHD_USE_SSL | mhdFlags, 
                                                    this->port, 
                                                    NULL, 
                                                    NULL, 
                                                    HttpServer::Callback, 
                                                    this, 
                                                    MHD_OPTION_HTTPS_MEM_KEY, 
                                                    this->sslKey.c_str(),
                                                    MHD_OPTION_HTTPS_MEM_CERT, 
                                                    this->sslCert.c_str(), 
                                                    MHD_OPTION_THREAD_POOL_SIZE, 
                                                    this->threads, 
                                                    MHD_OPTION_END );
            }
            catch( JsonRpcException &exception )
            {
                return false;
            }
        }
        else
        {
            this->mhdDaemon = MHD_start_daemon( mhdFlags, 
                                                this->port, 
                                                NULL, 
                                                NULL, 
                                                HttpServer::Callback, 
                                                this, 
                                                MHD_OPTION_THREAD_POOL_SIZE, 
                                                this->threads, 
                                                MHD_OPTION_END );
        }
		if( this->mhdDaemon != NULL )
		{
			this->isRunning = true;
		}
    }
	
	return this->isRunning;
}

bool HttpServer::StopListening()
{
    if( this->isRunning )
    {
        MHD_stop_daemon( this->mhdDaemon );
        this->isRunning = false;
    }

    return true;
}

bool HttpServer::SendResponse( const std::string &response, void *addInfo )
{
    struct mhd_ConnectionInfo *clientConnection = static_cast<struct mhd_ConnectionInfo*>( addInfo );
    struct MHD_Response *result = MHD_create_response_from_buffer( response.size(), ( void* )response.c_str(), MHD_RESPMEM_MUST_COPY );

    MHD_add_response_header( result, "Content-Type", "application/json" );
    MHD_add_response_header( result, "Access-Control-Allow-Origin", "*" );

    int return_ = MHD_queue_response( clientConnection->connection, clientConnection->code, result );
    MHD_destroy_response( result );

    return return_ == MHD_YES;
}

bool HttpServer::SendOptionsResponse( void *addInfo )
{
    struct mhd_ConnectionInfo *clientConnection = static_cast<struct mhd_ConnectionInfo*>( addInfo );
    struct MHD_Response *result = MHD_create_response_from_buffer( 0, NULL, MHD_RESPMEM_MUST_COPY );

    MHD_add_response_header( result, "Allow", "POST, OPTIONS" );
    MHD_add_response_header( result, "Access-Control-Allow-Origin", "*" );
    MHD_add_response_header( result, "Access-Control-Allow-Headers", "origin, content-type, accept" );
    MHD_add_response_header( result, "DAV", "1" );

    int return_ = MHD_queue_response( clientConnection->connection, clientConnection->code, result );
    MHD_destroy_response( result );

    return return_ == MHD_YES;
}

void HttpServer::SetUrlHandler( const std::string &url, IClientConnectionHandler *clientConnectionHandler )
{
    this->urlHandler[url] = clientConnectionHandler;
    this->SetHandler(NULL);
}

HttpServer::MicroHttpdResult HttpServer::Callback( void *cls, 
                                                   struct MHD_Connection *connection, 
                                                   const char *url,
                                                   const char *method,
                                                   const char *version,
                                                   const char *uploadData,
                                                   size_t *uploadDataSize,
                                                   void **con_cls ) 
{
    (void)version;
    if( *con_cls )
    {
        struct mhd_ConnectionInfo *clientConnection = new mhd_ConnectionInfo;
        clientConnection->connection = connection;
        clientConnection->httpServer = static_cast<HttpServer*>(cls);
        *con_cls = clientConnection;

        return MHD_YES;
    }

    struct mhd_ConnectionInfo *clientConnection = static_cast<struct mhd_ConnectionInfo*>(*con_cls);

    if( string( "POST" ) == method )
    {
        if( *uploadDataSize != 0 )
        {
            clientConnection->request.write( uploadData, *uploadDataSize );
            *uploadDataSize = 0;
            return MHD_YES;
        }
        else
        {
            string response;
            IClientConnectionHandler *clientConnectionHandler = clientConnection->httpServer->GetConnectionHandler( string( url ) );
            if( clientConnectionHandler == NULL )
            {
                clientConnection->code = MHD_HTTP_INTERNAL_SERVER_ERROR;
                clientConnection->httpServer->SendResponse( "No client connection handler found!", clientConnection );
            }
            else 
            {
                clientConnection->code = MHD_HTTP_OK;
                clientConnectionHandler->HandleRequest( clientConnection->request.str(), response );
                clientConnection->httpServer->SendResponse( response, clientConnection );
            }
        }    
    }
    else if( string( "OPTIONS" ) == method )
    {
        clientConnection->code = MHD_HTTP_OK;
        clientConnection->httpServer->SendOptionsResponse( clientConnection );
    }
    else
    {
        clientConnection->code = MHD_HTTP_METHOD_NOT_ALLOWED; 
        clientConnection->httpServer->SendResponse( "Not allowed HTTP method" );
    }

    if( clientConnection != nullptr )
    {
        delete clientConnection;
    }
    *con_cls = NULL;

    return MHD_YES;
}
