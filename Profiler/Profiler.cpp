#pragma warning(disable: 4996)

#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "profiler.h"

#define MAX_PROFILER (128)
#define MIN(num0, num1) ((num0) < (num1) ? (num0) : (num1))
#define MAX(num0, num1) ((num0) < (num1) ? (num1) : (num0))

typedef struct profile
{
	const TCHAR* tag_name;
	double sum_msec;
	double min_msec;
	double max_msec;
	unsigned int num_call;
} t_profile;

typedef struct profiler
{
	profile profile;
	LARGE_INTEGER start;
	BOOL is_profiling;
} t_profiler;

static t_profiler g_profilers[MAX_PROFILER];
static unsigned int g_num_profiler;

static t_profiler* get_profiler_or_null(const TCHAR* tag_name)
{
	for (unsigned int i = 0; i < g_num_profiler; ++i)
	{
		if (_tcscmp(g_profilers[i].profile.tag_name, tag_name) == 0)
		{
			return &g_profilers[i];
		}
	}
	return NULL;
}

void profile_begin(const TCHAR* tag_name)
{
	t_profiler* p_profiler = get_profiler_or_null(tag_name);

	if (p_profiler == NULL)
	{
		p_profiler = &g_profilers[g_num_profiler];

		t_profile* p_profile = &p_profiler->profile;
		{
			p_profile->tag_name = tag_name;
			p_profile->num_call = 0;
			p_profile->sum_msec = 0;
			p_profile->max_msec = DBL_MIN;
			p_profile->min_msec = DBL_MAX;
		}
		++g_num_profiler;
	}
	p_profiler->is_profiling = TRUE;

	QueryPerformanceCounter(&p_profiler->start);
}

void profile_end(const TCHAR* tag_name)
{
	LARGE_INTEGER freq;
	LARGE_INTEGER end;

	QueryPerformanceCounter(&end);
	QueryPerformanceFrequency(&freq);

	t_profiler* p_profiler = get_profiler_or_null(tag_name);

	if (p_profiler == NULL || !p_profiler->is_profiling)
	{
		return;
	}
	double elapsed_time = (end.QuadPart - p_profiler->start.QuadPart) / (double)freq.QuadPart * 1000000;

	t_profile* p_profile = &p_profiler->profile;
	{
		p_profile->sum_msec += elapsed_time;
		p_profile->min_msec = MIN(elapsed_time, p_profile->min_msec);
		p_profile->max_msec = MAX(elapsed_time, p_profile->max_msec);
		++p_profile->num_call;
	}

	p_profiler->is_profiling = false;
}

void print_profiles(const TCHAR* file_name)
{
	FILE* fp = _tfopen(file_name, _T("w"));
	{
		_ftprintf(fp, _T("%17s |%17s |%17s |%17s |%15s\n"), _T("Name"), _T("Average"), _T("Min"), _T("Max"), _T("Call"));
		for (unsigned int i = 0; i < g_num_profiler; ++i)
		{
			t_profile* p_profile = &g_profilers[i].profile;
			{
				const TCHAR* tag_name = p_profile->tag_name;
				double min = p_profile->min_msec;
				double max = p_profile->max_msec;
				double sum = p_profile->sum_msec;
				unsigned int num_call = p_profile->num_call;
				double average = (sum - max - min) / (num_call - 2);

				_ftprintf(fp, _T("%17s |%15.4fus |%15.4fus |%15.4fus |%15u\n"), tag_name, average, min, max, num_call);
			}
		}
	}
	fclose(fp);
}
