#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "../abstractthreadedserver.h"

namespace itsp
{
	class TcpSocketServer : public AbstractThreadedServer 
	{
		public:
			TcpSocketServer( const std::string &ipToBind, const unsigned int &port, size_t threadsCount = 1 );
			virtual ~TcpSocketServer();

			virtual bool InitializeListener();
			virtual int CheckForConnection();
			virtual void HandleConnection( int connection );

		protected:
			std::string ipToBind;
			unsigned int port;
			int socketFd;
			struct sockaddr_in address;

			bool WaitClientClose( const int &fd, const int &timeout = 100000 );
			int CloseByReset( const int &fd );
			int CleanClose( const int &fd );
	};
}

#endif // TCPSOCKETSERVER_H
