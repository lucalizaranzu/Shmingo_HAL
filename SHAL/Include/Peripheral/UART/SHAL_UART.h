/**
  ******************************************************************************
  * @file    SHAL_TIM.h
  * @author  Luca Lizaranzu
  * @brief   Relating to SHAL_UART and USART object abstractions
  ******************************************************************************
  */

#ifndef SHMINGO_HAL_SHAL_UART_H
#define SHMINGO_HAL_SHAL_UART_H

#include "SHAL_UART_REG.h"

class SHAL_UART{

    friend class UARTManager;
public:

    void init(UART_Pair pair);

    //begins Tx and Usart TODO either modify this function or add a new one that supports Rx
    void begin(uint32_t baudRate) volatile;

    //Sends a string
    void sendString(const char* s) volatile;

    //Sends a char
    void sendChar(char c) volatile;

private:

    SHAL_UART() = default; //Initializer for array

    //Creates a SHAL_UART based on a pair of two valid U(S)ART pins

    UART_Pair m_UARTPair = UART_Pair::INVALID;

};


#define UART(num) UARTManager::get(num)

class UARTManager{

public:

    static SHAL_UART& get(uint8_t uart);

    UARTManager() = delete;

private:

    inline static SHAL_UART m_UARTs[NUM_USART_LINES] = {};

};

#endif //SHMINGO_HAL_SHAL_UART_H
