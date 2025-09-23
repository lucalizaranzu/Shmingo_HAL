//
// Created by Luca on 9/21/2025.
//

#ifndef SHMINGO_HAL_SHAL_ADC_H
#define SHMINGO_HAL_SHAL_ADC_H

#include <cstdint>

#include "SHAL_CORE.h"
#include "SHAL_ADC_REG.h"

class SHAL_ADC {

    friend class ADCManager;

public:

    SHAL_Result init();

    SHAL_Result calibrate();

    /// Performs analog to digital conversion on a single channel, one time
    /// \param channel Channel to be converted
    /// \param time ADC_SampleTime - amount of clock cycles per conversion
    /// \return resulting value
    uint16_t singleConvertSingle(ADC_Channel channel, ADC_SampleTime time = ADC_SampleTime::C239);

    /// Performs analog to digital conversion on multiple channels, one time
    /// \param channels Pointer to an array of channels to convert
    /// \param numChannels Number of channels to convert
    /// \param result Pointer to store converted channel results in
    /// \param time ADC_SampleTime - amount of clock cycles per conversion
    void singleConvertSingle(ADC_Channel* channels, const int numChannels, uint16_t* result, ADC_SampleTime time = ADC_SampleTime::C239);



private:

    SHAL_ADC() = default;

    ADC_Key m_ADCKey = ADC_Key::INVALID;

};





class ADCManager{

public:

    static SHAL_ADC& get(ADC_Key key);

    ADCManager() = delete;

private:

    inline static SHAL_ADC m_ADCs[static_cast<uint8_t>(ADC_Key::NUM_ADC)] = {};

};

#endif //SHMINGO_HAL_SHAL_ADC_H
