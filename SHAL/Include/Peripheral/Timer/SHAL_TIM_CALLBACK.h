/**
  ******************************************************************************
  * @file    SHAL.h
  * @author  Luca Lizaranzu
  * @brief   Utilities for creating and populating the TIMER_KEY IRQ callback table
  * globally, see usage in SHAL_TIM.h. Created in use for singleton TIMER_KEY abstractions
  ******************************************************************************
  */

#ifndef SHAL_TIM_CALLBACK_H
#define SHAL_TIM_CALLBACK_H

#include "SHAL_CORE.h"
#include "SHAL_TIM_REG.h"

#define DEFINE_TIMER_IRQ(key, irq_handler)                \
extern "C" void irq_handler(void) {                       \
    auto tim_reg = getTimerRegister(key);                 \
    if (tim_reg->SR & TIM_SR_UIF) {                       \
        tim_reg->SR &= ~TIM_SR_UIF; /* clear flag */      \
        auto cb = timer_callbacks[static_cast<int>(key)]; \
        if (cb) cb();                                     \
    };                                                    \
};

typedef void (*TimerCallback)(); //Typedef for callback function

[[maybe_unused]] static TimerCallback timer_callbacks[static_cast<int>(Timer_Key::NUM_TIMERS)] = {nullptr}; //Timer IRQ Callback table

void registerTimerCallback(Timer_Key key, TimerCallback callback);

#endif //SHMINGO_HAL_SHAL_TIM_CALLBACK_H
