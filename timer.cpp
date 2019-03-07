#include "timer.h"

Timer::Timer()
{

}

double Timer::getDeltaTime()
{
    return deltaTime;
}

double Timer::getTotalTime()
{
    return totalTime;
}

void Timer::update()
{
    int currentTime = getMiliseconds();
    if (pre_time != 0)
    {
        deltaTime = (currentTime - pre_time) / 1000.0;
    }
    pre_time = currentTime;
    totalTime += deltaTime;
}


int Timer::getMiliseconds()
{
    timeb tb;
    ftime(&tb);
    int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
    return nCount;
}
