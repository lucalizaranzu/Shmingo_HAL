#include <cstdio>
#include "SHAL.h"

GPIO_Key gpios[6] = {
        GPIO_Key::A0,
        GPIO_Key::A1,
        GPIO_Key::A4,
        GPIO_Key::A5,
        GPIO_Key::A6,
        GPIO_Key::A7,
};

uint16_t vals[6] = {0,0,0,0,0,0};
uint8_t currentSensor = 0;

bool isAlarmBeeping = false;

bool prevIsCalibrateButtonHigh = false;

uint16_t sensorThresholds[6] = {4096,4096,4096,4096,4096,4096};


int cyclesPerPrint = 4;
int currentCycle = 0;

void getSensorData(){

    vals[currentSensor] = GPIOManager::get(gpios[currentSensor]).analogRead(SHAL_ADC_SampleTime::C8);

    if(currentSensor == 5 && currentCycle == cyclesPerPrint - 1){
        char buff[64];
        sprintf(buff, "%d, %d, %d, %d, %d, %d\r\n", vals[0],vals[1],vals[2],vals[3],vals[4],vals[5]);
        SHAL_UART2.sendString(buff);
    }

    currentSensor = (currentSensor + 1) % 6;
    currentCycle = (currentCycle + 1) % cyclesPerPrint;
}

void calibrateThresholds(){

    for(int i = 0; i < 6; i++){
        uint16_t sensorVal = GPIOManager::get(gpios[i]).analogRead(SHAL_ADC_SampleTime::C8);
        sensorThresholds[i] = sensorVal;
        SHAL_delay_ms(80);
    }
    char buff[80];
    sprintf(buff, "Thresholds: %d, %d, %d, %d, %d, %d\r\n", sensorThresholds[0],sensorThresholds[1],sensorThresholds[2],sensorThresholds[3],sensorThresholds[4],sensorThresholds[5]);
    SHAL_UART2.sendString(buff);
}



void PWMToggle(){

    if(!isAlarmBeeping){
        SHAL_TIM1.start();
    }
    else{
        SHAL_TIM1.stop();
    }
    isAlarmBeeping = !isAlarmBeeping;
}

void buttonHoldCallback(){
    PIN(B3).toggle();

    SHAL_TIM7.stop(); //Stop this timer

    SHAL_TIM2.stop(); //Stop reading from ADC
    calibrateThresholds();
    SHAL_TIM2.start(); //Restart value checks
}

int main() {

    SHAL_init();

    SHAL_UART2.init(UART_Pair_Key::Tx2A2_Rx2A3);
    SHAL_UART2.begin(115200);

    PIN(A0).setPinMode(PinMode::ANALOG_MODE);
    PIN(A1).setPinMode(PinMode::ANALOG_MODE);
    PIN(A4).setPinMode(PinMode::ANALOG_MODE);
    PIN(A5).setPinMode(PinMode::ANALOG_MODE);
    PIN(A6).setPinMode(PinMode::ANALOG_MODE);
    PIN(A7).setPinMode(PinMode::ANALOG_MODE);

    PIN(B0).setAlternateFunction(GPIO_Alternate_Function_Mapping::B0_TIM1CH2N);

    SHAL_TIM2.init(4000000,400);

    SHAL_TIM2.setCallbackFunc(getSensorData);
    SHAL_TIM2.enableInterrupt();
    SHAL_TIM2.start();

    SHAL_TIM1.init(300,999);
    SHAL_TIM1.setPWMMode(SHAL_Timer_Channel::CH2,SHAL_TIM_Output_Compare_Mode::PWMMode1,SHAL_Timer_Channel_Main_Output_Mode::Disabled,SHAL_Timer_Channel_Complimentary_Output_Mode::Polarity_Reversed);
    SHAL_TIM1.setPWMDutyCycle(499);
    SHAL_TIM1.start();

    SHAL_TIM6.init(4000000,400);
    SHAL_TIM6.setCallbackFunc(PWMToggle);
    SHAL_TIM6.enableInterrupt();
    SHAL_TIM6.start();

    SHAL_TIM7.init(4000000,6000);
    SHAL_TIM7.setCallbackFunc(buttonHoldCallback);
    SHAL_TIM7.enableInterrupt();
    //SHAL_TIM7.start();

    PIN(B6).setPinMode(PinMode::INPUT_MODE);
    PIN(B3).setPinMode(PinMode::OUTPUT_MODE); //Test
    PIN(B3).setLow();

    while (true) {
        //Retarded polling based button methods cause EXTI decided to not work on L432KC for some stupid reason at the last second
        if(!(PIN(B6).digitalRead() == 1)){
            //SHAL_UART2.sendString("High\r\n");
            if(!prevIsCalibrateButtonHigh){
                SHAL_TIM7.start();
            }
            prevIsCalibrateButtonHigh = true;
        }
        else{
            //SHAL_UART2.sendString("Low\r\n");
            if(prevIsCalibrateButtonHigh){
                //Button released
                SHAL_TIM7.stop();
            }
            prevIsCalibrateButtonHigh = false;
        }

    }
}