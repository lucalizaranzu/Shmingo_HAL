/**
  ******************************************************************************
  * @file    SHAL_TIM.h
  * @author  Luca Lizaranzu
  * @brief   Related to USART and UART abstractions
  ******************************************************************************
  */


#include "SHAL_UART.h"
#include "SHAL_GPIO.h"

UART::UART(const UART_Pair pair) : m_UARTPair(pair){
    SHAL_UART_Pair uart_pair = getUARTPair(pair); //Get the UART_PAIR information to be initialized

    //Get the GPIO pins for this UART setup
    GPIO_Key Tx_Key = uart_pair.TxKey; //Tx pin
    GPIO_Key Rx_Key = uart_pair.RxKey; //Rx pin

    GET_GPIO(Tx_Key).setPinMode(PinMode::ALTERNATE_FUNCTION_MODE);
    GET_GPIO(Rx_Key).setPinMode(PinMode::ALTERNATE_FUNCTION_MODE);

    GET_GPIO(Tx_Key).setAlternateFunction(uart_pair.TxAlternateFunctionMask);
    GET_GPIO(Rx_Key).setAlternateFunction(uart_pair.RxAlternateFunctionMask);

    SHAL_UART_ENABLE_REG pairUARTEnable = getUARTEnableReg(pair); //Register and mask to enable the UART channel

    *pairUARTEnable.reg |= pairUARTEnable.mask; //Enable UART line
}

void UART::begin(uint32_t baudRate) volatile {

    USART_TypeDef* usart = getUARTPair(m_UARTPair).USARTReg;

    usart->CR1 &= ~USART_CR1_UE; //Disable USART

    usart->CR1 = 0; //Clear USART config

    usart->CR1 = USART_CR1_TE | USART_CR1_RE; //Tx enable and Rx Enable

    usart->BRR = 8000000 / baudRate; //MAKE SURE ANY FUNCTION THAT CHANGES CLOCK UPDATES THIS! //TODO DO NOT HARDCODE THIS SHIT

    usart->CR1 |= USART_CR1_UE;

}

void UART::sendString(const char *s) volatile {
    while (*s) sendChar(*s++); //Send chars while we haven't reached end of s
}

void UART::sendChar(char c) volatile {

    USART_TypeDef* usart = getUARTPair(m_UARTPair).USARTReg;

    while(!(usart->ISR & USART_ISR_TXE)); //Wait for usart to finish what it's doing

    usart->TDR = c; //Send character
}



UART& UARTManager::get(UART_Pair pair) {

    //Reassign if pair doesn't match
    if(m_UARTs[getUARTChannel(pair)].m_UARTPair != pair) {
        m_UARTs[getUARTChannel(pair)] = UART(pair);
    }
    return m_UARTs[getUARTChannel(pair)];
}
