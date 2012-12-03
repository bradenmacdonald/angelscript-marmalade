// Properly set the target architecture (cannot be done in the MKB):
#ifdef I3D_ARCH_X86
	#define AS_X86
#elif I3D_ARCH_ARM
	#define AS_ARM
	#define _ARM_
#else
	#error Unsupported architecture. Fix the preinclude file.
#endif

// Fix missing fopen_s in Marmalade MSVC environment
#if defined(_MSC_VER) && _MSC_VER >= 1500 
	#include <stdio.h>
	inline int fopen_s(FILE** pFile, const char *filename, const char *mode) {
		*pFile = fopen(filename, mode);
		return 0;
	}
#endif
