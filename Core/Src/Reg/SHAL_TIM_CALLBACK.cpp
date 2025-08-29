//
// Created by Luca on 8/28/2025.
//

#include "SHAL_TIMER_CALLBACK.h"

void registerTimerCallback(Timer_Key key, TimerCallback callback){
    timer_callbacks[static_cast<int>(key)] = callback;
}