#include <Utils/Timer.h>

Timer::Timer() : mStartTicks(0), mPausedTicks(0), mPaused(false), mStarted(false)
{
    mStartTicks = 0;
    mPausedTicks = 0;
    mPaused = false;
    mStarted = false;
}

void Timer::start()
{
    mStarted = true;
    mPaused = false;

    mStartTicks = SDL_GetTicksNS();
    mPausedTicks = 0;
}

void Timer::stop()
{
    mStarted = false;
    mPaused = false;

    mStartTicks = 0;
    mPausedTicks = 0;
}

void Timer::pause()
{
    if (mStarted && !mPaused)
    {
        mPaused = true;

        mPausedTicks = SDL_GetTicksNS() - mStartTicks;
        mStartTicks = 0;
    }
}

void Timer::unpause()
{
    if (mStarted && mPaused)
    {
        mPaused = false;

        mStartTicks = SDL_GetTicksNS() - mPausedTicks;
        mPausedTicks = 0;
    }
}

Uint64 Timer::getTicksNS() const
{
    Uint64 time{0};

    if (mStarted)
    {
        if (mPaused)
        {
            time = mPausedTicks;
        }
        else
        {
            time = SDL_GetTicksNS() - mStartTicks;
        }
    }

    return time;
}

bool Timer::isStarted() const
{
    return mStarted;
}

bool Timer::isPaused() const
{
    return mPaused;
}
