//
// Created by Luca on 8/28/2025.
//

#include "SHAL_TIM_CALLBACK.h"

DEFINE_TIMER_IRQ(Timer_Key::S_TIM1, TIM1_IRQHandler)
DEFINE_TIMER_IRQ(Timer_Key::S_TIM2, TIM2_IRQHandler)
DEFINE_TIMER_IRQ(Timer_Key::S_TIM6, TIM6_IRQHandler)
DEFINE_TIMER_IRQ(Timer_Key::S_TIM7, TIM7_IRQHandler)
DEFINE_TIMER_IRQ(Timer_Key::S_TIM15, TIM15_IRQHandler)
DEFINE_TIMER_IRQ(Timer_Key::S_TIM16, TIM16_IRQHandler)

void registerTimerCallback(Timer_Key key, TimerCallback callback){
    timer_callbacks[static_cast<uint32_t>(key)] = callback;
}