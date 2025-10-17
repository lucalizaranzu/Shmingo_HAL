//
// Created by Luca on 8/29/2025.
//

#ifndef SHAL_GPIO_H
#define SHAL_GPIO_H

#include "SHAL_GPIO_REG.h"

#include <cassert>

#include "SHAL_EXTI_CALLBACK.h"
#include "SHAL_ADC.h"


//Abstraction of SHAL_GPIO registers
class SHAL_GPIO{

public:

    void toggle() volatile;

    //TODO replace stupid offset hack from APB
    void setHigh();
    void setLow();

    /// Uses the ADC to read an analog voltage value
    /// \param sampleTime The amount of clock cycles to use for the ADC
    /// \return ADC result
    uint16_t analogRead(SHAL_ADC_SampleTime sampleTime = SHAL_ADC_SampleTime::C8);

    void setPinMode(PinMode mode) volatile;

    void setAlternateFunction(GPIO_Alternate_Function AF) volatile;

    void setPinType(PinType type) volatile;

    void setOutputSpeed(OutputSpeed speed) volatile;

    void setInternalResistor(InternalResistorType type) volatile;


    void useAsExternalInterrupt(TriggerMode mode, EXTICallback callback);



private:

    friend class GPIOManager;

    explicit SHAL_GPIO(GPIO_Key key);
    SHAL_GPIO();

    GPIO_Key m_GPIO_KEY = GPIO_Key::INVALID;

};





//Init SHAL_GPIO for normal use
#define PIN_TO_KEY(name) GPIO_Key::name
#define PIN(name) GPIOManager::get(PIN_TO_KEY(name))

#define GET_GPIO(key) GPIOManager::get(key)

#define SET_ANALOGREAD_ADC(x) GPIOManager::setGPIOADC(x)

//Manages instances of SHAL_GPIO objects
class GPIOManager{

public:

    static SHAL_GPIO& get(GPIO_Key);

    static SHAL_ADC getGPIOADC(){ return m_GPIO_ADC;}

    static void setGPIOADC(SHAL_ADC adc){m_GPIO_ADC = adc;}

    GPIOManager() = delete;

private:

    inline static SHAL_GPIO m_gpios[AVAILABLE_PORTS][PINS_PER_PORT] = {{}};

    inline static SHAL_ADC m_GPIO_ADC = SHAL_ADC(1);

};


#endif //SHMINGO_HAL_SHAL_GPIO_H
