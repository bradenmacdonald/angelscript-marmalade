# AngelScript for Marmalade

This repository lets you use 
[AngelScript](http://www.angelcode.com/angelscript/) with
[Marmalade](http://www.madewithmarmalade.com). No modifications to the 
AngelScript code are used, and native calling conventions are available for both
x86 and ARM builds :-)

The AngelScript addons are compiled, and are available using the following 
include path convention:

```
#include <angelscript.h>
#include <angelscript/add_on/scriptstdstring.h>
```

# Building AngelScript

If you use the MKB file, the Marmalade build system will automatically download and
compile AngelScript. 

# Known Issues

## Function calls crash or pass garbage on OS X with clang/LLVM compiler

If function calls are not getting passed correctly, make sure that "dead code
stripping" is turned off in the project linker options. The dead code stripping
can sometimes remove the assembly code needed to use native calling conventions.
