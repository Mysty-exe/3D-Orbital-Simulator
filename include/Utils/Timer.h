#pragma once
#include <SDL3/SDL.h>

class Timer
{
public:
    Timer();

    void start();
    void stop();
    void pause();
    void unpause();

    Uint64 getTicksNS() const;

    bool isStarted() const;
    bool isPaused() const;

private:
    Uint64 mStartTicks;
    Uint64 mPausedTicks;

    bool mPaused;
    bool mStarted;
};
