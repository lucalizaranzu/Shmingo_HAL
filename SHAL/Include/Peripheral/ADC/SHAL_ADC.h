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

    SHAL_Result init(ADC_Key key);

    SHAL_Result calibrate();

    SHAL_Result configureResolution(SHAL_ADC_Resolution resolution);

    SHAL_Result configureAlignment(SHAL_ADC_Alignment alignment);

    /// Performs analog to digital conversion on a single channel, one time
    /// \param channel Channel to be converted
    /// \param time SHAL_ADC_SampleTime - amount of clock cycles per conversion
    /// \return resulting value
    uint16_t singleConvertSingle(SHAL_ADC_Channel channel, SHAL_ADC_SampleTime time = SHAL_ADC_SampleTime::C8);

    /// Performs analog to digital conversion on multiple channels, one time
    /// \param channels Pointer to an array of channels to convert
    /// \param numChannels Number of channels to convert
    /// \param result Pointer to store converted channel results in
    /// \param time SHAL_ADC_SampleTime - amount of clock cycles per conversion
    SHAL_Result multiConvertSingle(SHAL_ADC_Channel* channels, int numChannels, uint16_t* result, SHAL_ADC_SampleTime time = SHAL_ADC_SampleTime::C8);



private:

    SHAL_ADC() = default;

    ADC_Key m_ADCKey = ADC_Key::INVALID;

    //Checks to see if instance is initialized with a proper ADC peripheral tag
    bool isValid();

    //Enabled peripheral
    SHAL_Result enable();

    //Disables peripheral
    SHAL_Result disable();

    //Wake up ADC from initial deep sleep state
    SHAL_Result wakeFromDeepSleep();

    SHAL_Result startConversion();

    /// Adds an ADC channel to the conversion sequence
    /// \param channel Channel to add
    /// \param index Index to add channel to (ADC channel will be the nth channel to convert
    /// \return Result
    SHAL_Result addADCChannelToSequence(SHAL_ADC_Channel channel, uint32_t index);

    /// Sets the amount of ADC channels to convert
    /// \param amount Number of channels to convert
    /// \return
    SHAL_Result setADCSequenceAmount(uint32_t amount);

};


#define SHAL_ADC(x) ADCManager::getByIndex(x-1)

class ADCManager{

public:

    static SHAL_ADC& get(ADC_Key key);

    static SHAL_ADC& getByIndex(int index);


    ADCManager() = delete;

private:

    inline static SHAL_ADC m_ADCs[static_cast<uint8_t>(ADC_Key::NUM_ADC)] = {};

};

#endif //SHMINGO_HAL_SHAL_ADC_H
