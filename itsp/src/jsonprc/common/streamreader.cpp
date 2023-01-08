#include "streamreader.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace itsp;
using namespace std;

StreamReader::StreamReader( size_t bufferSize ) : bufferSize( bufferSize ), buffer( static_cast<char *>( malloc( bufferSize ) ) ) {}

StreamReader::~StreamReader() { free( buffer ); }

bool StreamReader::Read( std::string &target, int fd, char delimiter ) 
{
	ssize_t bytesRead;
	do 
	{
		bytesRead = read( fd, this->buffer, bufferSize );
		if ( bytesRead < 0 ) 
		{
			return false;
		} 
		else 
		{
			target.append(buffer, static_cast<size_t>( bytesRead ));
		}
	} while ( memchr( buffer, delimiter, bytesRead ) == NULL ); //(target.find(delimiter) == string::npos && bytesRead > 0);

	target.pop_back();
	return true;
}
