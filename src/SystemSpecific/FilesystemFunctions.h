#ifndef FILESYSTEM_FUNCTIONS_INCLUDED
#define FILESYSTEM_FUNCTIONS_INCLUDED

#include "Join.h"

extern bool FileExists (const char* name);
extern bool DirectoryExists (const char* name);
extern void CreateFile (const char* name);
extern void CreateDirectory (const char* name);
extern long GetMaxPathname ();
extern void RemoveFile (const char* name);
extern void RemoveDirectory (const char* name);

#endif /* FILESYSTEM_FUNCTIONS_INCLUDED */
