#ifndef STREAMREADER_H
#define STREAMREADER_H

#include <memory>
#include <string>

namespace itsp 
{
	class StreamReader 
	{
		public:
			StreamReader( size_t bufferSize );
			virtual ~StreamReader();
			bool Read( std::string &target, int fd, char delimiter );

		private:
			size_t bufferSize;
			char *buffer;
	};
}

#endif // STREAMREADER_H
