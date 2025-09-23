//
// Created by Luca on 9/21/2025.
//

#ifndef SHMINGO_HAL_SHAL_ADC_TYPES_H
#define SHMINGO_HAL_SHAL_ADC_TYPES_H

enum class ADC_Channel : uint32_t {
    CH1 = ADC_CHSELR_CHSEL1,
    CH2 = ADC_CHSELR_CHSEL2,
    CH3 = ADC_CHSELR_CHSEL3,
    CH4 = ADC_CHSELR_CHSEL4,
    CH5 = ADC_CHSELR_CHSEL5,
    CH6 = ADC_CHSELR_CHSEL6,
    CH7 = ADC_CHSELR_CHSEL7,
    CH8 = ADC_CHSELR_CHSEL8,
    CH9 = ADC_CHSELR_CHSEL9,
    CH10 = ADC_CHSELR_CHSEL10,
    CH11 = ADC_CHSELR_CHSEL11,
    CH12 = ADC_CHSELR_CHSEL12,
    CH13 = ADC_CHSELR_CHSEL13,
    CH14 = ADC_CHSELR_CHSEL14,
    CH15 = ADC_CHSELR_CHSEL15,
    CHTemp = ADC_CHSELR_CHSEL16,
    CHRef = ADC_CHSELR_CHSEL17,
    CHBat = ADC_CHSELR_CHSEL18
};

enum class ADC_SampleTime : uint32_t {
    C2          = 0x00, //1.5 cycles per sample
    C7          = 0x01, //7.5 cycles
    C13         = 0x02, //13.5 cycles
    C28         = 0x03, //28.5 cycles
    C41         = 0x04, //41.5 cycles
    C55         = 0x05, //55.5 cycles
    C71         = 0x06, //71.5 cycles
    C239        = 0x07  //239.5 cycles
};

#endif //SHMINGO_HAL_SHAL_ADC_TYPES_H
