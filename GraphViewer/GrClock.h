
#ifndef GR_CLOCK_H

#define GR_CLOCK_H

class GrClock
{
    bool bBack, bPause;
    long cur_tick, pre_clock, shift;
public:
    long get();
    void back();
    void begin();
    void pause();
    void forward();

    GrClock();
};

extern GrClock grClock;

#endif