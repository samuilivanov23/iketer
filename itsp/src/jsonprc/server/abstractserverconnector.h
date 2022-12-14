#ifndef ABSTRACTSERVERCONNECTOR_H
#define ABSTRACTSERVERCONNECTOR_H

#include "iclientconnectionhandler.h"
#include <string>

namespace itsp
{
    class AbstractServerConnector
    {
        public:
            AbstractServerConnector();
            virtual ~AbstractServerConnector();

            //Signals the connector to start listening for new requests
            //Returns FALSE if something is wrong, otherwise TRUE
            virtual bool StartListening() = 0;

            //Signals the connector to stop listening for new requests
            //Returns FALSE if something is wrong, otherwise TRUE
            virtual bool StopListening() = 0;

            //This is the main method of processing incoming requests
            void ProcessRequest( const std::string &request, std::string &response );

            void SetHandler( IClientConnectionHandler *clientConnectionHandler );
            IClientConnectionHandler *GetHandler();

        private:
            IClientConnectionHandler *clientConnectionHandler;
    };
}

#endif //ABSTRACTSERVERCONNECTOR_H
