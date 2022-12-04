#include "abstractserverconnector.h"
#include <cstdlib>
#include "../common/specificationwriter.h"

using namespace std;
using namespace itsp;

AbstractServerConnector::AbstractServerConnector() { this->clientConnectionHandler = NULL; }

AbstractServerConnector::~AbstractServerConnector() {}

void AbstractServerConnector::ProcessRequest( const std::string &request, const std::string &response )
{
    if( this->clientConnectionHandler != NULL ) { this->clientConnectionHandler->HandleRequest( requst, response ); }
}

void AbstractServerConnector::SetHandler( IClientConnectionHandler *clientConnectionHandler )
{
    this->clientConnectionHandler = clientConnectionHandler;
}

IClientConnectionHandler *AbstractServerConnector::GetHandler { return this->clientConnectionHandler; }
