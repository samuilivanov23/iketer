#ifndef STREAMWRITER_H
#define STREAMWRITER_H

#include <memory>
#include <string>

namespace itsp 
{
	class StreamWriter 
	{
		public:
			bool Write( const std::string &source, int fd );
	};
}

#endif // STREAMWRITER_H
