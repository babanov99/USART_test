#ifndef RCC_STM32F429_1
#define RCC_STM32F429_1
#include <stdint.h>
#include <stm32f429xx.h>
#define PLL_SOURCE 0b10
enum SOURCE_PLL
{
    HSI = 0,
    HSE = 1
};
enum error_PLL
{
    NICE = 0,
    ERROR_CANT_CALCULATE = 1,
    ERROR_CANT_ON_HSE = 2,
    ERROR_CANT_ON_PLL = 3,
    ERROR_MORE_THEN_I_CAN = 4
};

enum PLLP_data
{
    PLLP_2_mull = 0b00, //: PLLP = 2
    PLLP_4_mull = 0b01, //: PLLP = 4
    PLLP_6_mull = 0b10, //: PLLP = 6
    PLLP_8_mull = 0b11  //: PLLP = 8
};

struct PLL_mulls
{
    int16_t PLLN;
    uint16_t PLLM;
    enum PLLP_data PLLP;
};
enum error_PLL initCorePLL(enum SOURCE_PLL source_PLL, uint8_t HSE_Mhz, uint8_t needed_Mhz);
#endif