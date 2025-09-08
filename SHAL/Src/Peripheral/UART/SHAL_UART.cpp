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

    uint8_t Tx_Pin = getGPIORegister(Tx_Key).global_offset;
    uint8_t Rx_Pin = getGPIORegister(Rx_Key).global_offset;

    initGPIO(Tx_Key,PinMode::ALTERNATE_FUNCTION_MODE); //Initialize Tx GPIO with alternate function (initializes GPIO port as well)
    initGPIO(Rx_Key,PinMode::ALTERNATE_FUNCTION_MODE); //Initialize Rx GPIO with alternate function

    //Determine which AFR register (high or low) to write depending on pin
    uint8_t TxAFR = Tx_Pin < 8 ? 0 : 1; //Use AFR[0] if pin < 8, AFR[1] if pin >= 8
    uint8_t RxAFR = Rx_Pin < 8 ? 0 : 1;

    /*Apply Alternate Function masks to the AFR registers for each GPIO to enable alternate functions
     * The AFR register for GPIO_Typedef* is actually two registers - a low reg and high reg.
     * The low reg handles pins 0-7, and the high reg handles 8-15.
     * Each pin gets 4 bits in the register for AFR0 - AFR7. Hence 8 * 4 = 32 bits.
     * Each AFR is a different function, look at the DATASHEET (not reference manual) to find these alternate functions
     */
    getGPIORegister(Tx_Key).reg->AFR[TxAFR] |= getAFMask(uart_pair.TxMask) << (4 * (Tx_Pin % 8));
    getGPIORegister(Rx_Key).reg->AFR[RxAFR] |= getAFMask(uart_pair.RxMask) << (4 * (Rx_Pin % 8));

    SHAL_UART_ENABLE_REG pairUARTEnable = getUARTEnableReg(pair); //Register and mask to enable the UART channel

    *pairUARTEnable.reg |= pairUARTEnable.mask; //Enable UART line
}

void UART::begin(uint32_t baudRate) {

    USART_TypeDef* usart = getUARTPair(m_UARTPair).USARTReg;

    usart->CR1 &= ~USART_CR1_UE; //Disable USART

    usart->CR1 = 0; //Clear USART config

    usart->CR1 = USART_CR1_TE | USART_CR1_RE; //Tx enable and Rx Enable

    usart->BRR = 8000000 / baudRate; //MAKE SURE ANY FUNCTION THAT CHANGES CLOCK UPDATES THIS!

    usart->CR1 |= USART_CR1_UE;

}

void UART::sendString(const char *s) {
    while (*s) sendChar(*s++); //Send chars while we haven't reached end of s
}

void UART::sendChar(char c) {

    USART_TypeDef* usart = getUARTPair(m_UARTPair).USARTReg;

    while(!(usart->ISR & USART_ISR_TXE)); //Wait for usart to finish what it's doing

    usart->TDR = c; //Send character
}




UART& UARTManager::get(UART_Pair pair) {

    //Always reassign since we could be changing to different pins for some reason
    m_UARTs[getUARTChannel(pair)] = UART(pair);

    return m_UARTs[getUARTChannel(pair)];
}
