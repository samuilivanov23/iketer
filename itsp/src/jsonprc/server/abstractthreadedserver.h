#ifndef ABSTRACTTHREADEDSERVER_H
#define ABSTRACTTHREADEDSERVER_H

#include "abstractserverconnector.h"
#include "threadpool.h"
#include <memory>
#include <thread>

namespace itsp
{
	class AbstractThreadedServer : public AbstractServerConnector 
	{
		public:
			AbstractThreadedServer( size_t threads );
			virtual ~AbstractThreadedServer();
			virtual bool StartListening();
			virtual bool StopListening();

		protected:
			// Initializes sockets and file descriptors
			virtual bool InitializeListener() = 0;

			// Non-blocking operation that polls for a new connection
			// Returns a handle which is passed to the HandleConnection()
			virtual int CheckForConnection() = 0;

			// Handles connection info for a given handle returned by CheckForConnection()
			virtual void HandleConnection( int connection ) = 0;

		private:
			bool isRunning;
			std::unique_ptr<std::thread> listenerThread;
			ThreadPool threadPool;
			size_t threadsCount;
			void ListenLoop();
	};
}

#endif // ABSTRACTTHREADEDSERVER_H
