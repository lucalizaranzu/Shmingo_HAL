/**
  ******************************************************************************
  * @file    SHAL_TIM.h
  * @author  Luca Lizaranzu
  * @brief   Declarations of TIMER_KEY related objects
  ******************************************************************************
  */

#ifndef SHAL_TIM_H
#define SHAL_TIM_H

#include "SHAL_TIM_REG.h"
#include "SHAL_TIM_CALLBACK.h"

#include <array>

class Timer {
    friend class TimerManager;
public:

    /// Initializes a timer
    /// \param prescaler The amount of times the base clock has to cycle before the timer adds one to the count
    /// \param autoReload The number of timer counts before the count is reset and IRQ is called
    void init(uint32_t prescaler, uint32_t autoReload);

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
        registerTimerCallback(m_key, callback);
    }

private:

    explicit Timer(Timer_Key t);
    Timer();

    Timer_Key m_key;

};


#define getTimer(timer_key) TimerManager::get(timer_key)
#define TIM(num) TimerManager::getTimerFromIndex(num)

//Manages all timers so user does not have to personally initialize
class TimerManager{
public:

    static Timer& get(Timer_Key);

    static Timer& getTimerFromIndex(uint8_t index){return timers[index];}

    TimerManager() = delete;

private:
    inline static Timer timers[static_cast<int>(Timer_Key::NUM_TIMERS)] = {};
};

#endif