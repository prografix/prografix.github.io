
#include "Windows.h"

namespace
{
    class Timer
    {
    public:
        double  coef;
        Timer ()
        {
            __int64  freq;
            QueryPerformanceFrequency ( (LARGE_INTEGER *) &freq);
            coef = 1. / double ( freq );
        }
    };

    const Timer timer;
}

double timeInSec()
{
    __int64 count;
   QueryPerformanceCounter((LARGE_INTEGER *) &count);
   return timer.coef * double ( count  );
}