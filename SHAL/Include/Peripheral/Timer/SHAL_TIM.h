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
    void init(uint16_t prescaler, uint16_t autoReload);

    /// Simple function to set a timer in basic PWM mode
    /// @param channel Channel to output on
    /// @param prescaler Divider from sysclock
    /// @param autoReload Counter value to reset at
    /// @param captureCompareThreshold PWM trigger value (duty cycle = this / autoReload)
    void configurePWM(SHAL_Timer_Channel channel, uint16_t prescaler, uint16_t autoReload, uint16_t captureCompareThreshold);



    void configureOneshot(SHAL_Timer_Channel channel, uint16_t prescaler, uint16_t autoReload, uint16_t captureCompareThreshold);

    //Starts the counter
    void start();

    //Stops the counter
    void stop();

    //Set prescaler value
    void setPrescaler(uint16_t presc) const;

    //Set auto reload register
    void setARR(uint16_t arr) const;

    //Enable interrupts
    void enableInterrupt();

    //Capture Compare Functions
    void setCaptureCompareValue(SHAL_Timer_Channel channel, uint16_t value);
    void enableChannel(SHAL_Timer_Channel channel, SHAL_Timer_Channel_Main_Output_Mode mainOutputMode, SHAL_Timer_Channel_Complimentary_Output_Mode complimentaryOutputMode);
    void setOutputCompareMode(SHAL_Timer_Channel channel, SHAL_TIM_Output_Compare_Mode outputCompareMode);

    //Set TIMER_KEY IRQ callback function
    void setCallbackFunc(TimerCallback callback) const {
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