#include <cstdio>
#include "SHAL.h"


#define NUM_CHANNELS 8

SHAL_ADC_Channel channels[NUM_CHANNELS] = {
        SHAL_ADC_Channel::CH5,
        SHAL_ADC_Channel::CH6,
        SHAL_ADC_Channel::CH8,
        SHAL_ADC_Channel::CH9,
        SHAL_ADC_Channel::CH10,
        SHAL_ADC_Channel::CH11,
        SHAL_ADC_Channel::CH12,
        SHAL_ADC_Channel::CH7
};

uint16_t vals[NUM_CHANNELS] = {0,0,0,0,0,0,0,0};
uint8_t currentSensor = 0;

bool isAlarmBeeping = false;
bool prevIsCalibrateButtonHigh = false;

uint16_t sensorThresholds[NUM_CHANNELS] = {};

int buzzer_beepCount = 0;
bool isBeepingForCalibration = false;

int cyclesPerPrint = 2;
int currentCycle = 0;

bool areSensorRequirementsMetCurrent = false;
bool areSensorRequirementsMetPrevious = false;

void getSensorData(){

    vals[currentSensor] = SHAL_ADC1.singleConvertSingle(channels[currentSensor]);

    if(currentSensor == (NUM_CHANNELS - 1) && currentCycle == cyclesPerPrint - 1){
        char buff[125];
        sprintf(buff, "5:%d,6:%d,8:%d,9:%d,10:%d,11:%d,12:%d,7:%d\r\n", vals[0],vals[1],vals[2],vals[3],vals[4],vals[5],vals[6],vals[7]);
        SHAL_UART2.sendString(buff);
    }

    currentSensor = (currentSensor + 1) % NUM_CHANNELS;
    currentCycle = (currentCycle + 1) % cyclesPerPrint;
}

void startBeeping(){
    SHAL_TIM1.start();
    SHAL_TIM6.start();
}

void stopBeeping(){
    SHAL_TIM1.stop();
    SHAL_TIM6.stop();
    isAlarmBeeping = false;
    isBeepingForCalibration = false;
}

void calibrateThresholds(){

    for(int i = 0; i < 6; i++){
        uint16_t sensorVal = SHAL_ADC1.singleConvertSingle(channels[currentSensor]);
        sensorThresholds[i] = (sensorVal / 5) * 4;
    }
}


void PWMToggle(){

    if(isBeepingForCalibration && buzzer_beepCount > 2){
        isBeepingForCalibration = false;
        buzzer_beepCount = 0;
        SHAL_TIM6.stop(); //Reset timer 6
        SHAL_TIM1.stop(); //Stop buzzer
        SHAL_TIM6.init(4000000,400);
    }

    if(!isAlarmBeeping){
        SHAL_TIM1.start();
        buzzer_beepCount++;
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

    buzzer_beepCount = 0;
    isBeepingForCalibration = true;

    SHAL_TIM6.init(4000000,80);
    SHAL_TIM6.start();

    calibrateThresholds();
    SHAL_TIM1.start();

    SHAL_TIM2.start(); //Restart value checks
}

int main() {

    SHAL_init();

    //SHAL_UART2.init(UART_Pair_Key::Tx2A2_Rx2A3);
    //SHAL_UART2.begin(115200);

    PIN(A0).setPinMode(PinMode::ANALOG_MODE);
    PIN(A1).setPinMode(PinMode::ANALOG_MODE);
    PIN(A2).setPinMode(PinMode::ANALOG_MODE);
    PIN(A3).setPinMode(PinMode::ANALOG_MODE);
    PIN(A4).setPinMode(PinMode::ANALOG_MODE);
    PIN(A5).setPinMode(PinMode::ANALOG_MODE);
    PIN(A6).setPinMode(PinMode::ANALOG_MODE);
    PIN(A7).setPinMode(PinMode::ANALOG_MODE);


    SHAL_TIM2.init(4000000,400);

    SHAL_TIM2.setCallbackFunc(getSensorData);
    SHAL_TIM2.enableInterrupt();
    SHAL_TIM2.start();

    PIN(B0).setAlternateFunction(GPIO_Alternate_Function_Mapping::B0_TIM1CH2N);

    SHAL_TIM1.init(0,2400);
    SHAL_TIM1.setPWMMode(SHAL_Timer_Channel::CH2,SHAL_TIM_Output_Compare_Mode::PWMMode1,SHAL_Timer_Channel_Main_Output_Mode::Disabled,SHAL_Timer_Channel_Complimentary_Output_Mode::Polarity_Reversed);
    SHAL_TIM1.setPWMDutyCycle(900);
    //PIN(B0).setPinMode(PinMode::OUTPUT_MODE);

    SHAL_TIM6.init(4000000,400);
    SHAL_TIM6.setCallbackFunc(PWMToggle);
    SHAL_TIM6.enableInterrupt();

    SHAL_TIM7.init(4000000,4500);
    SHAL_TIM7.setCallbackFunc(buttonHoldCallback);
    SHAL_TIM7.enableInterrupt();

    PIN(B6).setPinMode(PinMode::INPUT_MODE);
    PIN(B3).setPinMode(PinMode::OUTPUT_MODE); //Test
    PIN(B3).setLow();

    while (true) {
        //Retarded polling based button methods cause EXTI decided to not work on L432KC for some stupid reason at the last second
        if(!(PIN(B6).digitalRead() == 1)){
            areSensorRequirementsMetCurrent = true;

            if(!areSensorRequirementsMetPrevious){
                startBeeping();
            }
            /*
            if(!prevIsCalibrateButtonHigh){
                SHAL_TIM7.start();
            }
            prevIsCalibrateButtonHigh = true;
             */

        }
        else{
            areSensorRequirementsMetCurrent = false;
            if(areSensorRequirementsMetPrevious){
                stopBeeping();
            }

            /*
            if(prevIsCalibrateButtonHigh){
                //Button released
                SHAL_TIM7.stop();
            }
            prevIsCalibrateButtonHigh = false;
             */
        }

        areSensorRequirementsMetPrevious = areSensorRequirementsMetCurrent;
    }
}