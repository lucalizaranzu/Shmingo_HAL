//
// Created by Luca on 8/29/2025.
//

#ifndef SHMINGO_HAL_SHAL_GPIO_REG_F072XB_H
#define SHMINGO_HAL_SHAL_GPIO_REG_F072XB_H

#include <stm32f072xb.h>
#include <cassert>

#define AVAILABLE_GPIO \
    X(A0) X(A1) X(A2) X(A3) X(A4) X(A5) X(A6) X(A7) X(A8) X(A9) X(A10) X(A11) X(A12) X(A13) X(A14) X(A15) \
    X(B0) X(B1) X(B2) X(B3) X(B4) X(B5) X(B6) X(B7) X(B8) X(B9) X(B10) X(B11) X(B12) X(B13) X(B14) X(B15) \
    X(C0) X(C1) X(C2) X(C3) X(C4) X(C5) X(C6) X(C7) X(C8) X(C9) X(C10) X(C11) X(C12) X(C13) X(C14) X(C15)

//Build enum map of available GPIO pins
enum class GPIO_Key {
    #define X(key) key,
        AVAILABLE_GPIO
    #undef X
    NUM_GPIO,
    INVALID
};

constexpr volatile GPIO_TypeDef* getGPIORegister(GPIO_Key g) {
    switch(g) {
        case GPIO_Key::A0:
        case GPIO_Key::A1:
        case GPIO_Key::A2:
        case GPIO_Key::A3:
        case GPIO_Key::A4:
        case GPIO_Key::A5:
        case GPIO_Key::A6:
        case GPIO_Key::A7:
        case GPIO_Key::A8:
        case GPIO_Key::A9:
        case GPIO_Key::A10:
        case GPIO_Key::A11:
        case GPIO_Key::A12:
        case GPIO_Key::A13:
        case GPIO_Key::A14:
        case GPIO_Key::A15:
            return GPIOA;
        case GPIO_Key::B0:
        case GPIO_Key::B1:
        case GPIO_Key::B2:
        case GPIO_Key::B3:
        case GPIO_Key::B4:
        case GPIO_Key::B5:
        case GPIO_Key::B6:
        case GPIO_Key::B7:
        case GPIO_Key::B8:
        case GPIO_Key::B9:
        case GPIO_Key::B10:
        case GPIO_Key::B11:
        case GPIO_Key::B12:
        case GPIO_Key::B13:
        case GPIO_Key::B14:
        case GPIO_Key::B15:
            return GPIOB;
        case GPIO_Key::C0:
        case GPIO_Key::C1:
        case GPIO_Key::C2:
        case GPIO_Key::C3:
        case GPIO_Key::C4:
        case GPIO_Key::C5:
        case GPIO_Key::C6:
        case GPIO_Key::C7:
        case GPIO_Key::C8:
        case GPIO_Key::C9:
        case GPIO_Key::C10:
        case GPIO_Key::C11:
        case GPIO_Key::C12:
        case GPIO_Key::C13:
        case GPIO_Key::C14:
        case GPIO_Key::C15:
            return GPIOC;
        case GPIO_Key::INVALID:
        case GPIO_Key::NUM_GPIO:
            assert(false);
            return nullptr; //Unreachable
    }
    __builtin_unreachable();
}

#endif //SHMINGO_HAL_SHAL_GPIO_REG_F072XB_H
