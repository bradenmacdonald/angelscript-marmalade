// Properly set the target architecture (cannot be done in the MKB):
#ifdef I3D_ARCH_X86
	#define AS_X86
#elif I3D_ARCH_ARM
	#define AS_ARM
	#define _ARM_
#else
	#error Unsupported architecture. Fix the preinclude file.
#endif
