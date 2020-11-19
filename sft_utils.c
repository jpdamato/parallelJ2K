

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/times.h>
#endif /* _WIN32 */
#include "sft_utils.h"

double sft_clock(void)
{
#ifdef _WIN32
	/* _WIN32: use QueryPerformance (very accurate) */
    LARGE_INTEGER freq , t ;
    /* freq is the clock speed of the CPU */
    QueryPerformanceFrequency(&freq) ;
	/* cout << "freq = " << ((double) freq.QuadPart) << endl; */
    /* t is the high resolution performance counter (see MSDN) */
    QueryPerformanceCounter ( & t ) ;
    return ( t.QuadPart /(double) freq.QuadPart ) ;
#else
	/* Unix or Linux: use resource usage */
    struct rusage t;
    double procTime;
    /* (1) Get the rusage data structure at this moment (man getrusage) */
    getrusage(0,&t);
    /* (2) What is the elapsed time ? - CPU time = User time + System time */
	/* (2a) Get the seconds */
    procTime = t.ru_utime.tv_sec + t.ru_stime.tv_sec;
    /* (2b) More precisely! Get the microseconds part ! */
    return ( procTime + (t.ru_utime.tv_usec + t.ru_stime.tv_usec) * 1e-6 ) ;
#endif
}

