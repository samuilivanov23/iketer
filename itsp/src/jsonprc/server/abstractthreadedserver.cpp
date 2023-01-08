#include "abstractthreadedserver.h"

using namespace itsp;
using namespace std;

AbstractThreadedServer::AbstractThreadedServer( size_t threadsCount ) : isRunning( false ), threadPool( threadsCount ), threadsCount( threadsCount ) {}

AbstractThreadedServer::~AbstractThreadedServer() { this->StopListening(); }

bool AbstractThreadedServer::StartListening()
{
	if( this->isRunning ) return false;

	if( !this->InitializeListener() ) return false;

	this->isRunning = true;
	this->listenerThread = unique_ptr<thread>( new thread( &AbstractThreadedServer::ListenLoop, this ) );

	return true;
}

bool AbstractThreadedServer::StopListening()
{
	if( !this->isRunning ) return false;

	this->isRunning = false;
	this->listenerThread->join();

	return true;
}

void AbstractThreadedServer::ListenLoop()
{
	while( this->isRunning )
	{
		int connection = this->CheckForConnection();

		if( connection > 0 )
		{
			if( this->threadsCount > 0 )
			{
				this->threadPool.enqueue( &AbstractThreadedServer::HandleConnection, this, connection );
			}
			else
			{
				this->HandleConnection(connection);
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}
