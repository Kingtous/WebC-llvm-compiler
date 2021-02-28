//
// Created by 金韬 on 2021/2/26.
//

#ifdef WIN32
#include <Windows.h>
#include <time.h>
#else

#include <sys/time.h>

#endif

#ifdef WIN32
int gettimeofday(struct timeval* tp, void* tzp)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year = wtm.wYear - 1900;
    tm.tm_mon = wtm.wMonth - 1;
    tm.tm_mday = wtm.wDay;
    tm.tm_hour = wtm.wHour;
    tm.tm_min = wtm.wMinute;
    tm.tm_sec = wtm.wSecond;
    tm.tm_isdst = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;
    return (0);
}
#endif

/**
 * 获取ms数
 * @return ms
 */
long __getms() {
    struct timeval tp;
    gettimeofday(&tp, 0);
    long ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return ms;
}
