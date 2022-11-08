#pragma once

#ifndef SERVER_H
#define SERVER_H

#include "config.h"
#include "dispatcher.h"
#include "msgpack.hpp"

#include "detail/pimpl.h"

namespace itsp 
{
    namespace detail 
    {
        class ServerSession;
    }

    // The server maintains a registry of function bindings that it uses to
    // dispatch calls. It also takes care of managing worker threads and TCP
    // connections.
    // The server does not start listening right after construction in order
    // to allow binding functions before that. Use the `run` or `async_run`
    // functions to start listening on the port.
    // This class is not copyable, but moveable.
    class Server 
    {
        public:
            // Constructs a server that listens on the localhost on the
            // specified port.
            //
            // <param port> The port number to listen on.
            explicit Server( uint16_t port );
            
            // Move constructor. This is implemented by calling the
            // move assignment operator.
            //
            // <param other> The other instance to move from.
            Server( Server&& other );
            
            // Constructs a server that listens on the specified address on
            // the specified port.
            //
            // <param address> The address to bind to. This only works if oee of your
            // network adapaters control the given address.
            // <param port> The port number to listen on.
            Server( std::string const &address, uint16_t port );
            
            // Destructor.
            //
            // When the server is destroyed, all ongoin sessions are closed gracefully.
            ~Server();

            // Move assignment operator.
            //
            // <param other> The other instance to move from.
            // return The result of the assignment.
            Server& operator=( Server&& other );
            
            // Starts the server loop. This is a blocking call.
            //
            // First and foremost, running the event loop causes the server to start
            // listening on the specified port. Also, as connections are established
            // and calls are made by clients, the server executes the calls as part
            // of this call. This means that the handlers are executed on the thread
            // that calls `run`. Reads and writes are initiated by this function
            // internally as well.
            void Run();

            //Starts the server loop on one or more threads. This is a
            // non-blocking call.
            //
            // This function behaves similarly to `run`, except the event loop is
            // optionally started on different threads. Effectively this sets up a
            // worker thread pool for the server. Handlers will be executed on one
            // of the threads.
            //
            // <param worker_threads> The number of worker threads to start.
            void AsyncRun( std::size_t workerThreads=1 );
            
            // Binds a functor to a name so it becomes callable via RPC.
            //
            // This function template accepts a wide range of callables. The arguments
            // and return types of these callables should be serializable by msgpack.
            // `bind` effectively generates a suitable, light-weight compile-time
            // wrapper for the functor.
            //
            // <param name >The name of the functor.
            // <param func> The functor to bind.
            // <param F> The type of the functor.
            template <typename F> void Bind( std::string const &name, F function )
            {
                disp_->bind(name, function);
            }

            // Unbinds a functor binded to a name.
            //
            // This function removes already binded function from RPC Ccallable functions
            //
            // <param name> The name of the functor.
            void Unbind( std::string const &name )
            {
                disp_->unbind(name);
            }

            //  //! \brief Returns all binded names
            //
            // This function returns a list of all names which functors are binded to
            //
            // <param name> The name of the functor.
            std::vector<std::string> Names() const 
            {
                return disp_->names();
            }

            // Sets the exception behavior in handlers. By default,
            // handlers throwing will crash the server. If suppressing is on,
            // the server will try to gather textual data and return it to
            // the client as an error response.
            // note=***Setting this flag only affects subsequent connections.***
            void SuppressExceptions( bool suppress );

            // Stops the server
            // note=***This should not be called form worker threads.***
            void Stop();

            // Returns the port 
            unsigned short Port() const;

            // Closes all sessions
            void CloseSessions();
            
            // Closes a specific session
            void CloseSession( std::shared_pointer<detail::ServerSession> const& serverSession );

        private:
            ITSP_DECLARE_PIMPL()
            std::shared_pointer<detail::Dispatcher> disp_;
    };
}

#endif /* End of include guard: SERVER_H */
