/**
  ******************************************************************************
  * @file    SHAL_TIM.h
  * @author  Luca Lizaranzu
  * @brief   Declarations of TIMER_KEY related objects
  ******************************************************************************
  */

#ifndef SHAL_TIM_H
#define SHAL_TIM_H

#include "SHAL_TIM_REG_F072xB.h"
#include "SHAL_TIM_CALLBACK.h"

#include <array>

class Timer {
    friend class TimerManager;
public:

    //Starts the counter
    void start();

    //Stops the counter
    void stop();

    //Set prescaler value
    void setPrescaler(uint16_t presc);

    //Set auto reload register
    void setARR(uint16_t arr);

    //Enable interrupts
    void enableInterrupt();

    //Set TIMER_KEY IRQ callback function
    void setCallbackFunc(TimerCallback callback){
        registerTimerCallback(TIMER_KEY, callback);
    }

private:

    explicit Timer(Timer_Key t);
    Timer();

    Timer_Key TIMER_KEY;

};

#define getTimer(timer_key) TimerManager::get(timer_key);

//Manages all timers so user does not have to personally initialize
class TimerManager{
public:

    static Timer& get(Timer_Key);
    TimerManager() = delete;

private:
    inline static Timer timers[static_cast<int>(Timer_Key::NUM_TIMERS)] = {};
};

#endif