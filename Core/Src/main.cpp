#include "SHAL.h"
#include "stm32f0xx.h"

volatile int prev_button = false;
volatile int curr_button = false;

extern "C" void TIM2_IRQHandler(void){
	if(TIM2->SR & TIM_SR_UIF){
		TIM2->SR &= ~TIM_SR_UIF;
		GPIOA->ODR ^= (1 << 4);
	}
}

extern "C" void EXTI0_1_IRQHandler(void) {
    if (EXTI->PR & (1 << 0)) {   //Check pending flag
        EXTI->PR |= (1 << 0);    //Clear it by writing 1
        GPIOA->ODR ^= (1 << 5);
    }
}

int main() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable SYSCFG clock (needed for EXTI)

    TIM2->EGR |= TIM_EGR_UG;   //Force update to load PSC/ARR

    GPIOA->MODER &= ~(0b11 << (4 * 2));
    GPIOA->MODER |= (0b1 << (4 * 2));

    GPIOA->MODER &= ~(0x3 << (5 * 2));
    GPIOA->MODER |=  (0x1 << (5 * 2));

    GPIOB->MODER &= ~(0x3 << (0 * 2));
    GPIOB->MODER |=  (0x0 << (0 * 2));

    TIM2->PSC = 8000 - 1; //8MHz base, prescaler
    TIM2->ARR = 500 - 1; //500ms, auto reload register

    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0; // Clear EXTI0 mapping
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB; // Map PA0 -> EXTI0

    EXTI->IMR |= (1 << 0);   // Unmask EXTI0
    EXTI->RTSR |= (1 << 0);  // Trigger on rising edge


    TIM2->DIER |= TIM_DIER_UIE; //Interrupt register
    TIM2->CR1 |= TIM_CR1_CEN; //Counter enable


    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_EnableIRQ(EXTI0_1_IRQn); // EXTI lines 0 and 1 share an IRQ vector

    __enable_irq();

    while (true) {
    	__WFI();
    }
}
