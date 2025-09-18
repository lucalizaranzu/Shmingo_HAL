//
// Created by Luca on 9/7/2025.
//

#ifndef SHAL_UART_TYPES_H
#define SHAL_UART_TYPES_H

#include "SHAL_CORE.h"
#include "SHAL_GPIO_REG.h"



//Represents a pair of pins usable for USART Tx + Rx in combination, and their alternate function mapping
struct SHAL_UART_Pair{
    USART_TypeDef* USARTReg;
    GPIO_Key TxKey;
    GPIO_Key RxKey;
    GPIO_Alternate_Function TxAlternateFunctionMask;
    GPIO_Alternate_Function RxAlternateFunctionMask;
};

struct SHAL_UART_ENABLE_REG{
    volatile uint32_t* reg;
    uint32_t mask;
};

#endif //SHMINGO_HAL_SHAL_UART_TYPES_H
