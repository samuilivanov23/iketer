#ifndef HTTPSERVERCONNECTOR_H
#define HTTPSERVERCONNECTOR_H

#include <sys/types.h>
#include <stdint.h>
#include <stdarg.h>

#if defined( _WIN32 ) && !defined( __CYGWIN__ )
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#if defined( _MSC_FULL_VER ) && !defined( _SSIZE_T_DEFINED )
		#define _SSIZE_T_DEFINED
		typedef intptr_t ssize_t;
	#endif // !_SSIZE_T_DEFINED */
	#else
		#include <arpa/inet.h>
		#include <netdb.h>
		#include <sys/socket.h>
		#include <sys/time.h>
		#include <unistd.h>
#endif

#include "../abstractserverconnector.h"
#include <map>
#include <microhttpd.h>

namespace itsp
{
  
    //This class provides HTTP Server, based on libmicrohttp module.
    //NB: This class always sends HTTP Status 200 OK, even though a JSON-RPC Error might have occured
    //Always check the JSON-RPC Error Header
    class HttpServer : public AbstractServerConnector
    {
        public:
            HttpServer( uint16_t port, const std::string &sslCert = "", const std::string &sslKey = "", uint32_t threads = 50 );
            ~HttpServer();

            //Binds the server to localhost only and deactivates TLS settings
            HttpServer &BindToLocalhost();
            virtual bool StartListening();
            virtual bool StopListening();
            virtual bool SendResponse( const std::string &response, void *addInfo = NULL );
            virtual bool SendOptionsResponse( void *addInfo );
            void SetUrlHandler( const std::string &url, IClientConnectionHandler *clientConnectionHandler );
        
        private:
			#if MHD_VERSION >= 0x00097002
				typedef MHD_Result MicroHttpdResult;
			#else
				typedef int MicroHttpdResult;
			#endif
            int port;
            int threads;
            bool isRunning;
            std::string pathSSLCert;
            std::string pathSSLKey;
            std::string sslCert;
            std::string sslKey;
            struct MHD_Daemon *mhdDaemon;
            bool isBindLocalhost;
            std::map<std::string, IClientConnectionHandler*> urlHandler;
            struct sockaddr_in loopbackAddress;

            static MicroHttpdResult Callback( void *cls, 
                                                struct MHD_Connection *connection, 
                                                const char *url, 
                                                const char *method, 
                                                const char *version,
                                                const char *upload_data, 
                                                size_t *upload_data_size, 
                                                void **con_cls );
            IClientConnectionHandler *GetConnectionHandler( const std::string &url );
    };
}

#endif //HTTPSERVERCONNECTOR_H
