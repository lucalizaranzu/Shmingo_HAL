#ifndef SHAL_TIM_H
#define SHAL_TIM_H

#include "SHAL_TIM_REG.h"
#include "SHAL_TIM_CALLBACK.h"

class Timer {
public:

    explicit Timer(Timer_Key t);

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

    //Set timer IRQ callback function
    void setCallbackFunc(TimerCallback callback){
        registerTimerCallback(timer, callback);
    }

private:

    Timer_Key timer;
    volatile TIM_TypeDef* timer_reg;

};

#endif