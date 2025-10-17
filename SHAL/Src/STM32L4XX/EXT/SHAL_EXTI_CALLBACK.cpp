//
// Created by Luca on 9/3/2025.
//

#include "SHAL_EXTI_CALLBACK.h"


#if defined(STM32L412xx)
#elif defined(STM32L422xx)
#elif defined(STM32L431xx)
#elif defined(STM32L432xx)
DEFINE_EXTI_IRQ(0);
DEFINE_EXTI_IRQ(1);
DEFINE_EXTI_IRQ(2);
DEFINE_EXTI_IRQ(3);
DEFINE_EXTI_IRQ(4);
DEFINE_MULTI_EXTI_IRQ(5,9);
DEFINE_MULTI_EXTI_IRQ(10,15);
#elif defined(STM32L433xx)
#elif defined(STM32L442xx)
#elif defined(STM32L443xx)
#elif defined(STM32L451xx)
#elif defined(STM32L452xx)
#elif defined(STM32L462xx)
#elif defined(STM32L471xx)
#elif defined(STM32L475xx)
#elif defined(STM32L476xx)
#elif defined(STM32L485xx)
#elif defined(STM32L486xx)
#elif defined(STM32L496xx)
#elif defined(STM32L4A6xx)
#elif defined(STM32L4P5xx)
#elif defined(STM32L4Q5xx)
#elif defined(STM32L4R5xx)
#elif defined(STM32L4R7xx)
#elif defined(STM32L4R9xx)
#elif defined(STM32L4S5xx)
#elif defined(STM32L4S7xx)
#elif defined(STM32L4S9xx)
 #error "Please select first the target STM32L4xx device used in your application (in stm32f0xx.h file)"
#endif


//Link function to EXTI line
void registerEXTICallback(GPIO_Key key, EXTICallback callback){
    EXTI_callbacks[getGPIORegister(key).global_offset] = callback;
}
