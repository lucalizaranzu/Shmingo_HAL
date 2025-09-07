//
// Created by Luca on 9/6/2025.
//

#ifndef SHMINGO_HAL_SHAL_GPIO_TYPES_H
#define SHMINGO_HAL_SHAL_GPIO_TYPES_H

#include "SHAL_CORE.h"


struct SHAL_EXTIO_Register{
    volatile uint32_t* EXT_ICR;
    uint32_t mask;
    IRQn_Type IRQN;
};

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
    uint8_t TxPinNumber;
    uint8_t RxPinNumber;
    volatile uint32_t* TxReg;
    volatile uint32_t* RxReg;
    AF_Mask TxMask;
    AF_Mask RxMask;
};

#endif //SHMINGO_HAL_SHAL_GPIO_TYPES_H
