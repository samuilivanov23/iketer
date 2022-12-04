#ifndef CLIENTCONNECTIONHANDLER_H
#define CLIENTCONNECTIONHANDLER_H

#include <string>

namespace itsp
{
    class Procedure;
    class IClientConnectionHandler
    {
        public:
            virtual ~IClientConnectionHandler();
            virtual void HandleRequest( std::string &request, std::string &response ) = 0;
    };

    class IProtocolHandler : public IClientConnectionHandler
    {
        public:
            virtual ~IProtocolHandler() {}
            virtual void AddProcedure( const Procedure &procedure ) = 0;
    };
}

#endif //CLIENTCONNECTIONHANDLER_H
