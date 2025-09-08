#include "SHAL.h"
#include "stm32f0xx.h"


volatile GPIO* blueLED = nullptr;
volatile GPIO* greenLED = nullptr;
volatile UART* uart2;

void c3Interrupt(){
    greenLED->toggle();
}

void tim2Handler(){
    blueLED->toggle();
}

int main() {

    uart2 = &getUART(UART_Pair::Tx2A2_Rx2A3);

    uart2->begin(115200);

    useGPIOAsInterrupt(GPIO_Key::C3,TriggerMode::RISING_EDGE,c3Interrupt);

    Timer timer2 = getTimer(Timer_Key::S_TIM2);

    blueLED = &initGPIO(GPIO_Key::A4, PinMode::OUTPUT_MODE);
    greenLED = &initGPIO(GPIO_Key::A5, PinMode::OUTPUT_MODE);

    timer2.setPrescaler(8000 - 1);
    timer2.setARR(1500 - 1);
    timer2.setCallbackFunc(tim2Handler);
    timer2.start();

    while (true) {
    	__WFI();
    }
}
