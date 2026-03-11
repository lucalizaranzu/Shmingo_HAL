#include <cstdio>
#include "SHAL.h"

#define SER PIN(B10)
#define RCLK PIN(B9)
#define SRCLK PIN(B8)

void scanOutputs(uint8_t num_outputs) {

    for (size_t i = 0; i < num_outputs + 1; i++) {
        if (i == 0) {
            SER.setHigh();
        }
        else {
            SER.setLow();
        }
        RCLK.setHigh();
        SRCLK.setHigh();
        RCLK.setLow();
        SRCLK.setLow();
    }
}

int main() {

    SHAL_init();

    PIN(A8).setPinMode(PinMode::ALTERNATE_FUNCTION_MODE);

    PIN(A8).setAlternateFunction(GPIO_Alternate_Function::AF2);

    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    SER.setPinMode(PinMode::OUTPUT_MODE);
    RCLK.setPinMode(PinMode::OUTPUT_MODE);
    SRCLK.setPinMode(PinMode::OUTPUT_MODE);

    SHAL_TIM1.init(48,100);
    SHAL_TIM1.configurePWM(SHAL_Timer_Channel::CH1, 4800, 100, 20);
    SHAL_TIM1.start();

    SER.setLow();
    RCLK.setLow();
    SRCLK.setLow();

    uint8_t numDisplays = 6;
    uint8_t count = 0;

    while (true) {

        if (count == 0) {
            SER.setLow();
        }
        else {
            SER.setHigh();
        }

        count++;

        RCLK.setHigh();
        SRCLK.setHigh();
        RCLK.setLow();
        SRCLK.setLow();
        if (count == numDisplays) {
            count = 0;
        }
        SHAL_delay_us(50);
    }
}
