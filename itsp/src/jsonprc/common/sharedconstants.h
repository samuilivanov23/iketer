#ifndef SHAREDCONSTANTS_H
#define SHAREDCONSTANTS_H

#ifdef __linux__
#include <linux/limits.h>
#endif

//Line Feed character
#define DEFAULT_DELIMITER_CHAR char(0x0A)

// Default size of read/write buffer size
#define DEFAULT_BUFFER_SIZE 1024

#endif // SHAREDCONSTANTS_H
