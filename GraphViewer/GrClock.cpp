
#include "GrClock.h"
#include "time.h"

GrClock::GrClock() : bBack(false), bPause(false)
{
    pre_clock = clock();
    cur_tick = 0;
}

long GrClock::get()
{
    if ( bPause == false )
    {
        long cur_clock = clock();
        long cl = cur_clock - pre_clock;
        pre_clock = cur_clock;
        if ( bBack )
        {
            cur_tick -= cl;
            if ( cur_tick < 0 ) cur_tick = 0;
        }
        else
        {
            cur_tick += cl;
        }
    }
    return cur_tick;
}

void GrClock::back()
{
    if ( bPause )
    {
        bPause = false;
        pre_clock = clock();
    }
    bBack = true;
}

void GrClock::begin()
{
    pre_clock = clock();
    cur_tick = 0;
}

void GrClock::pause()
{
    bPause = true;
}

void GrClock::forward()
{
    if ( bPause )
    {
        bPause = false;
        pre_clock = clock();
    }
    bBack = false;
}

GrClock grClock;