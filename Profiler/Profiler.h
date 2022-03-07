#ifndef PROFILER_H
#define PROFILER_H

#include <tchar.h>

#ifdef PROFILE
#define PROFILE_BEGIN(tag_name) profile_begin(tag_name);
#define PROFILE_END(tag_name)  profile_end(tag_name)
#define PROFILES_PRINT(file_name) print_profiles(file_name)
#else
#define PROFILE_BEGIN(tag_name)
#define PROFILE_END(tag_name)
#define PROFILES_PRINT(file_name)
#endif /* PROFILE */

void profile_begin(const TCHAR* tag_name);
void profile_end(const TCHAR* tag_name);
void print_profiles(const TCHAR* file_name);

#endif /* PROFILER_H */