//
// Created by Luca on 9/3/2025.
//

#include "SHAL_EXTI_CALLBACK.h"

#if defined(STM32F030x6)
#elif defined(STM32F030x8)
#elif defined(STM32F031x6)
#elif defined(STM32F038xx)
#elif defined(STM32F042x6)
#elif defined(STM32F048xx)
#elif defined(STM32F051x8)
#elif defined(STM32F058xx)
#elif defined(STM32F070x6)
#elif defined(STM32F070xB)
#elif defined(STM32F071xB)
#elif defined(STM32F072xB)

DEFINE_MULTI_EXTI_IRQ(0,1);
DEFINE_MULTI_EXTI_IRQ(2,3);
DEFINE_MULTI_EXTI_IRQ(4,15);

#elif defined(STM32F078xx)
#include "stm32f078xx.h"
#elif defined(STM32F091xC)
  #include "stm32f091xc.h"
#elif defined(STM32F098xx)
  #include "stm32f098xx.h"
#elif defined(STM32F030xC)
  #include "stm32f030xc.h"
#else
 #error "Please select first the target STM32F0xx device used in your application (in stm32f0xx.h file)"
#endif


//Link function to EXTI line
void registerEXTICallback(GPIO_Key key, EXTICallback callback){
    EXTI_callbacks[getGPIORegister(key).global_offset] = callback;
}
