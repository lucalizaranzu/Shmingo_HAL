#include "SHAL.h"
#include "stm32f0xx.h"

extern "C" void EXTI0_1_IRQHandler(void) {
    if (EXTI->PR & (1 << 0)) {   //Check pending flag
        EXTI->PR |= (1 << 0);    //Clear it by writing 1
        GPIOA->ODR ^= (1 << 5);
    }
}

void tim2Handler(){
    GPIOA->ODR ^= (1 << 4);
}

int main() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    Timer timer2 = getTimer(Timer_Key::S_TIM2);

    timer2.setPrescaler(8000 - 1);
    timer2.setARR(1500 - 1);
    timer2.setCallbackFunc(tim2Handler);
    timer2.start();


    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //Enable SYSCFG clock (needed for EXTI)

    GPIOA->MODER &= ~(0b11 << (4 * 2));
    GPIOA->MODER |= (0b1 << (4 * 2));

    GPIOA->MODER &= ~(0x3 << (5 * 2));
    GPIOA->MODER |=  (0x1 << (5 * 2));

    GPIOB->MODER &= ~(0x3 << (0 * 2));
    GPIOB->MODER |=  (0x0 << (0 * 2));

    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0; // Clear EXTI0 mapping
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB; // Map PA0 -> EXTI0

    EXTI->IMR |= (1 << 0);   // Unmask EXTI0
    EXTI->RTSR |= (1 << 0);  // Trigger on rising edge

    NVIC_EnableIRQ(EXTI0_1_IRQn); // EXTI lines 0 and 1 share an IRQ vector

    __enable_irq();

    while (true) {
    	__WFI();
    }
}
