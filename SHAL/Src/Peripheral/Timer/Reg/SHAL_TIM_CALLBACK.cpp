//
// Created by Luca on 8/28/2025.
//

#include "SHAL_TIM_CALLBACK.h"

DEFINE_TIMER_IRQ(Timer_Key::S_TIM1, TIM1_BRK_UP_TRG_COM_IRQHandler)
DEFINE_TIMER_IRQ(Timer_Key::S_TIM2, TIM2_IRQHandler)
DEFINE_TIMER_IRQ(Timer_Key::S_TIM3, TIM3_IRQHandler)
DEFINE_TIMER_IRQ(Timer_Key::S_TIM14, TIM14_IRQHandler)
DEFINE_TIMER_IRQ(Timer_Key::S_TIM15, TIM15_IRQHandler)
DEFINE_TIMER_IRQ(Timer_Key::S_TIM16, TIM16_IRQHandler)
DEFINE_TIMER_IRQ(Timer_Key::S_TIM17, TIM17_IRQHandler)

void registerTimerCallback(Timer_Key key, TimerCallback callback){
    timer_callbacks[static_cast<int>(key)] = callback;
}