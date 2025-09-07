//
// Created by Luca on 9/7/2025.
//

#ifndef SHMINGO_HAL_SHAL_UART_TYPES_H
#define SHMINGO_HAL_SHAL_UART_TYPES_H

#include "SHAL_CORE.h"
#include "SHAL_GPIO_REG.h"

enum class AF_Mask : uint8_t{
    AF0 = 0x00,
    AF1 = 0x01,
    AF2 = 0x02,
    AF3 = 0x03,
    AF4 = 0x04,
    AF5 = 0x05,
    AF6 = 0x06,
    AF7 = 0x07
};

//Represents a pair of pins usable for USART Tx + Rx in combination, and their alternate function mapping
struct SHAL_UART_Pair{

    USART_TypeDef* USARTReg;
    GPIO_Key TxKey;
    GPIO_Key RxKey;
    AF_Mask TxMask;
    AF_Mask RxMask;
};

struct SHAL_UART_ENABLE_REG{
    volatile uint32_t* USART_EN_Reg;
    uint32_t USART_EN_Mask;
};

#endif //SHMINGO_HAL_SHAL_UART_TYPES_H
