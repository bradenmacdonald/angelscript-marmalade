// Fix missing fopen_s in Marmalade MSVC environment
#if defined(_MSC_VER) && _MSC_VER >= 1500 
	#include <stdio.h>
	inline int fopen_s(FILE** pFile, const char *filename, const char *mode) {
		*pFile = fopen(filename, mode);
		return 0;
	}
#endif
