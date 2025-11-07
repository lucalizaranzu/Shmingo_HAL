/**
  ******************************************************************************
  * @file    SHAL_TIM.h
  * @author  Luca Lizaranzu
  * @brief   Related to USART and SHAL_UART abstractions
  ******************************************************************************
  */


#include "SHAL_UART.h"
#include "SHAL_GPIO.h"

void SHAL_UART::init(const UART_Pair_Key pair){

    m_UARTPair = pair;

    SHAL_UART_Pair uart_pair = getUARTPair(pair); //Get the UART_PAIR information to be initialized

    //Get the SHAL_GPIO pins for this SHAL_UART setup
    GPIO_Key Tx_Key = uart_pair.TxKey; //Tx pin
    GPIO_Key Rx_Key = uart_pair.RxKey; //Rx pin

    GET_GPIO(Tx_Key).setPinMode(PinMode::ALTERNATE_FUNCTION_MODE);
    GET_GPIO(Rx_Key).setPinMode(PinMode::ALTERNATE_FUNCTION_MODE);

    GET_GPIO(Tx_Key).setAlternateFunction(uart_pair.TxAlternateFunctionMask);
    GET_GPIO(Rx_Key).setAlternateFunction(uart_pair.RxAlternateFunctionMask);

    SHAL_UART_Enable_Register pairUARTEnable = getUARTEnableReg(pair); //Register and mask to enable the SHAL_UART channel

    *pairUARTEnable.reg |= pairUARTEnable.mask; //Enable SHAL_UART line


}

void SHAL_UART::begin(uint32_t baudRate) volatile {

    USART_TypeDef* usart = getUARTPair(m_UARTPair).USARTReg;

    usart->CR1 &= ~USART_CR1_UE; //Disable USART

    usart->CR1 = 0; //Clear USART config

    usart->CR1 = USART_CR1_TE | USART_CR1_RE; //Tx enable and Rx Enable

    usart->BRR = 8000000 / baudRate; //MAKE SURE ANY FUNCTION THAT CHANGES CLOCK UPDATES THIS! //TODO DO NOT HARDCODE THIS SHIT

    usart->CR1 |= USART_CR1_UE;

}

void SHAL_UART::sendString(const char *s) volatile {
    while (*s) sendChar(*s++); //Send chars while we haven't reached end of s
}

void SHAL_UART::sendChar(char c) volatile {

    USART_TypeDef* usart = getUARTPair(m_UARTPair).USARTReg;

    while(!(usart->ISR & USART_ISR_TXE)); //Wait for usart to finish what it's doing

    usart->TDR = c; //Send character
}



SHAL_UART& UARTManager::get(uint8_t uart) {

    if(uart > NUM_USART_LINES - 1){
        assert(false);
        //Memory fault
    }

    return m_UARTs[uart];
}
