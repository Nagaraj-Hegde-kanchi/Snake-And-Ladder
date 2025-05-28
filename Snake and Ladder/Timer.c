#ifndef TIMER_H_                               
#define TIMER_H_
#ifndef RAYLIB_H_
    #define RAYLIB_H_
    #include "raylib.h"
#endif
#ifndef STD_H_
    #define STD_H_
    #include <stdio.h>
#endif
typedef struct Timer
{
    double LifeTime;
}Timer;

void StartTimer(Timer *timer, float LifeTime);
bool IsTimerDone(Timer *timer);
void UpdateTimer(Timer *timer );
#endif

void StartTimer(Timer *timer, float LifeTime){
    if(timer!=NULL){
        timer->LifeTime=LifeTime;
    }
}

void UpdateTimer(Timer *timer ){
    if (timer!=NULL)
    {
        timer->LifeTime -=GetFrameTime();
    }
}

bool IsTimerDone(Timer *timer){
    if (timer!=NULL)
    {
        return (timer->LifeTime) <= 0;
    }
    return true;
}