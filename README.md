# AngelScript for Marmalade

This repository lets you use 
[AngelScript](http://www.angelcode.com/angelscript/) with
[Marmalade](http://www.madewithmarmalade.com). No modifications to the 
AngelScript code are used, and native calling conventions are available for both
x86 and ARM builds :-)

## Using AngelScript

Binaries are includes for x86 Windows, x86 Mac, and ARM. The binaries include
AngelScript and all addons. Simply add this project as a subproject in your MKB
file, and include the AngelScript headers using the following include path
convention:

```
#include <angelscript.h>
#include <angelscript/add_on/scriptstdstring.h>
```

A **"Hello world" example** is included in the `example` folder.

## Building AngelScript

If you use the MKB file, the Marmalade build system will automatically download
and compile AngelScript. 

## Known Issues

### Cannot build .S files using MSVC

To compile with Visual Studio under certain versions of Marmalade, you may need
to comment out one line of the MKB file - see note in the MKB file.

### Function calls crash or pass garbage when built with XCode

If function calls are not getting passed correctly, make sure that "dead code
stripping" is turned off in the project linker options. The dead code stripping
can sometimes remove the assembly code needed to use native calling conventions.

In theory, the `no-strip` MKB option would fix this, but it does not seem to
affect the XCode project; therefore, this build setting must be manually
changed.
