#include <cstdio>
#include "SHAL.h"

int main() {

    SHAL_init();

    PIN(A8).setPinMode(PinMode::ALTERNATE_FUNCTION_MODE);

    PIN(A8).setAlternateFunction(GPIO_Alternate_Function::AF2);

    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    SHAL_TIM1.init(48,100);
    SHAL_TIM1.setOutputCompareMode(SHAL_Timer_Channel::CH1,SHAL_TIM_Output_Compare_Mode::PWMMode1);
    SHAL_TIM1.enableChannel(SHAL_Timer_Channel::CH1,SHAL_Timer_Channel_Main_Output_Mode::Polarity_Reversed,SHAL_Timer_Channel_Complimentary_Output_Mode::Disabled);
    SHAL_TIM1.setCaptureCompareValue(SHAL_Timer_Channel::CH1, 5);
    SHAL_TIM1.start();
}
