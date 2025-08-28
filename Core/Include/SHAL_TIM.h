#ifndef SHAL_TIM_H
#define SHAL_TIM_H

#include "SHAL_TIM_REG.h"



class Timer {
public:

    explicit Timer(Timer_Key t);

    //Starts the counter
    void start();

    //Stops the counter
    void stop();

    void setPrescaler(uint16_t presc);

    void setARR(uint16_t arr);

    void enableInterrupt();

private:

    Timer_Key timer;
    volatile TIM_TypeDef* timer_reg;

};

#endif