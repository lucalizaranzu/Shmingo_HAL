/**
  ******************************************************************************
  * @file    SHAL_TIM.h
  * @author  Luca Lizaranzu
  * @brief   Callbacks for external interrupts
  ******************************************************************************
  */

#ifndef SHMINGO_HAL_SHAL_EXTI_CALLBACK_H
#define SHMINGO_HAL_SHAL_EXTI_CALLBACK_H

#include "SHAL_CORE.h"
#include "SHAL_GPIO_REG_F072xB.h"

#define DEFINE_EXTI_IRQ(EXTI_Channel)                           \
extern "C" void EXTI##EXTI_Channel##_IRQHandler(void) {         \
    if (EXTI->PR & (1 << EXTI_Channel)) {                   \
        EXTI->PR |= (1 << EXTI_Channel); /*clear flag */    \
        auto cb = EXTI_callbacks[EXTI_Channel];                 \
        if (cb) cb();                                           \
    };                                                          \
};

#define DEFINE_MULTI_EXTI_IRQ(EXTI_Channel_Min, EXTI_Channel_Max)                   \
extern "C" void EXTI##EXTI_Channel_Max##_##EXTI_Channel_Min##_IRQHandler(void) {    \
    for(uint8_t line = EXTI_Channel_Min; line <= EXTI_Channel_Max; line++){         \
        if (EXTI->PR & (1 << line)) {                                   \
            EXTI->PR |= (1 << line); /*clear flag */                    \
            auto cb = EXTI_callbacks[line];                                 \
            if (cb) cb();                                                           \
        };                                                                          \
    }                                                                               \
};

typedef void (*EXTICallback)(); //Typedef for callback function

[[maybe_unused]] static EXTICallback EXTI_callbacks[NUM_EXTI_LINES] = {nullptr}; //Timer IRQ Callback table

void registerEXTICallback(GPIO_Key key, EXTICallback callback);

#endif //SHMINGO_HAL_SHAL_EXTI_CALLBACK_H
