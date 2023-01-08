#include "tcpsocketserver.h"
#include "../../common/sharedconstants.h"
#include "../../common/streamwriter.h"
#include "../../common/streamreader.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace itsp;
using namespace std;

TcpSocketServer::TcpSocketServer( const std::string &ipToBind, const unsigned int &port, size_t threadsCount ) : AbstractThreadedServer( threadsCount ), ipToBind( ipToBind ), port( port ) {}

TcpSocketServer::~TcpSocketServer() 
{
	shutdown( this->socketFd, 2 );
	close( this->socketFd );
}

bool TcpSocketServer::InitializeListener() 
{
	this->socketFd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( this->socketFd < 0 ) 
	{
		return false;
	}

	fcntl( this->socketFd, F_SETFL, FNDELAY );
	int reuseAddr = 1;
	setsockopt( this->socketFd, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof( reuseAddr ) );

	/* start with a clean address structure */
	memset( &( this->address ), 0, sizeof( struct sockaddr_in ) );

	this->address.sin_family = AF_INET;
	inet_aton( this->ipToBind.c_str(), &( this->address.sin_addr ) );
	this->address.sin_port = htons( this->port );

	if ( ::bind( this->socketFd, reinterpret_cast<struct sockaddr *>( &( this->address ) ), sizeof( struct sockaddr_in ) ) != 0 ) 
	{
		return false;
	}

	if ( listen( this->socketFd, 5 ) != 0 )
   	{
		return false;
	}
	
	return true;
}

int TcpSocketServer::CheckForConnection() 
{
	struct sockaddr_in connectionAddress;
	memset( &connectionAddress, 0, sizeof( struct sockaddr_in ) );
	socklen_t addressLength = sizeof( connectionAddress );
	return accept( this->socketFd, reinterpret_cast<struct sockaddr *>( &( connectionAddress )), &addressLength );
}

void TcpSocketServer::HandleConnection( int connection )
{
	StreamReader reader (DEFAULT_BUFFER_SIZE);
	string request, response;

	reader.Read( request, connection, DEFAULT_DELIMITER_CHAR );
	this->ProcessRequest( request, response );

	response.append( 1, DEFAULT_DELIMITER_CHAR );
	StreamWriter writer;
	writer.Write( response, connection );

	CleanClose( connection );
}

bool TcpSocketServer::WaitClientClose( const int &fd, const int &timeout )
{
	bool ret = false;
	int i = 0;
	while( ( recv( fd, NULL, 0, 0 ) && i < timeout ) )
	{
		usleep( 1 );
		++i;
		ret = true;
	}

	return ret;
}

int TcpSocketServer::CloseByReset( const int &fd )
{
	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 0;
	
	int ret = setsockopt( fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof( so_linger ) );
	if( ret != 0 )
	{
		return ret;
	}
	
	return close( fd );
}


int TcpSocketServer::CleanClose( const int &fd )
{
	if( WaitClientClose( fd ) )
	{
		return close( fd );
	}
	else
	{
		return CloseByReset( fd );
	}
}
