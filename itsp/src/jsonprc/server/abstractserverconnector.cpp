#include "abstractserverconnector.h"
#include <cstdlib>
#include "../common/specificationwriter.h"

using namespace std;
using namespace itsp;

AbstractServerConnector::AbstractServerConnector() { this->handler = NULL; }

AbstractServerConnector::AbstractServerConnector() {}

void AbstractServerConnector::ProcessReuqest( const std::string &request, const std::string &response )
{
    if( this->connectionHandler != NULL ) { this->connectionHandler->HandleRequest( requst, response ); }
}

void AbstractServerConnector::SetHandler( IConnectionHandler *connectionHandler )
{
    this->connectionHandler = connectionHandler;
}

IConnectionHandler *AbstractServerConnector::GetHandler { return this->connectionHandler; }
