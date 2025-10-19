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

struct SHAL_UART_Enable_Register{
    volatile uint32_t* reg;
    uint32_t mask;
};

struct SHAL_UART_Control_Register_1 {
    volatile uint32_t* reg;
    uint32_t usart_enable_mask;
    uint32_t transmit_enable_mask;
    uint32_t receive_enable_mask;
};

struct SHAL_UART_Baud_Rate_Generation_Register {
    volatile uint32_t* reg;
};

struct SHAL_UART_Transmit_Data_Register {
    volatile uint16_t* reg;
};

struct SHAL_UART_ISR_FIFO_Disabled {
    volatile uint32_t* reg;
    uint32_t transmit_data_register_empty_mask;
};



#endif //SHMINGO_HAL_SHAL_UART_TYPES_H
