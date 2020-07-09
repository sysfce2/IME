#include "Clock.h"
#include <ctime>

Utility::Clock::Clock(){
    timeWhenClockWasLastReset_ = getProcessTimeInSeconds();
}

double Utility::Clock::getProcessTimeInSeconds(){
    clock_t time = clock();
    return static_cast<double>(time)/CLOCKS_PER_SEC;
}

double Utility::Clock::getElapsedTimeInSeconds(){
    return getProcessTimeInSeconds() - timeWhenClockWasLastReset_;
}

double Utility::Clock::restart(){
    auto timeElasped = getElapsedTimeInSeconds();
    timeWhenClockWasLastReset_ = getProcessTimeInSeconds();
    return timeElasped;
}