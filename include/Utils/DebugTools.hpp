#pragma once

// This prints traces as mangled function names. This command makes life easier:
// ./a2 2>&1 | grep a1 | tr '()+' ' ' | awk '{ printf "%s\n", $2; }'
// Then pipe that to c++-filt (which isn't on Alamode!) or use
// http://demangler.com.
#define trace() trace_helper(__FILE__, __LINE__, __FUNCTION__)
void trace_helper(const char *file, int line, const char *func);

// Error checking function which we can turn off later, if we need to, to catch
// generic errors: errno, OpenGL, and (maybe?) FMOD.
#define check_error() check_helper(__FILE__, __LINE__)
void check_helper(const char *file, int line);
#define glChk check_error
