#include "RCC_Stm32F428Zi.h"

struct PLL_mulls *calculatePLL(uint8_t HSE_Mhz, uint8_t needed_Mhz);

/// @brief Функция для инициализации частоты процессора
/// @note ВАЖНО! Тут не настраивается частота для USB и I2C. Здесь только SYSCLOCK
/// @param source_PLL HSE или HSI
/// @param HSE_Mhz Частота внешнего цварца
/// @param needed_Mhz Требуемая частота
/// @return Состояние о процессоре
enum error_PLL initCorePLL(enum SOURCE_PLL source_PLL, uint8_t HSE_Mhz, uint8_t needed_Mhz)
{
    volatile struct PLL_mulls *data;
    if (needed_Mhz > 130)
    {
        return ERROR_MORE_THEN_I_CAN;
    }
    // верим, что питание тут 3.3 вольта
    FLASH->ACR &= 0b111;
    if (needed_Mhz < 30)
    {
        __NOP();
    }
    else if (needed_Mhz < 60)
        FLASH->ACR |= 1;
    else if (needed_Mhz < 90)
        FLASH->ACR |= 2;
    else if (needed_Mhz < 120)
        FLASH->ACR |= 3;
    else
        FLASH->ACR |= 4;
    volatile uint32_t counter = 0;
    RCC->PLLCFGR |= (source_PLL << 22);
    RCC->PLLCFGR |= (0b1111 << 24);
    if (source_PLL == HSE)
    {
        RCC->CR |= (1 << 18);
        RCC->CR |= (1 << 16);
        while (!(RCC->CR & (1 << 17)))
        {
            counter++;
            if (counter == 100000)
            {
                return ERROR_CANT_ON_HSE;
            }
        }
    }
    data = calculatePLL(HSE_Mhz, needed_Mhz);
    if (data->PLLN == -1)
        return ERROR_CANT_CALCULATE;
    else
    {
        RCC->PLLCFGR |= (data->PLLN) << 6 | (data->PLLP << 16) | (data->PLLM << 0);
        RCC->CR |= (1 << 24);
        counter = 0;
        while (!(RCC->CR & (1 << 25)))
        {
            counter++;
            if (counter == 10000000)
            {
                return ERROR_CANT_ON_PLL;
            }
        }
        counter = 0;
        RCC->CFGR |= (PLL_SOURCE << 0);
        while (((RCC->CFGR & (0b11 << 2)) >> 2) != 0b10)
        {
            counter++;
            if (counter == 10000000)
            {
                return ERROR_CANT_ON_PLL;
            }
        }
        return NICE;
    }
    return ERROR_CANT_ON_PLL;
}
struct PLL_mulls *calculatePLL(uint8_t HSE_Mhz, uint8_t needed_Mhz)
{
    /*
        f(VCO clock) = f(PLL clock input) × (PLLN / PLLM)
        f(PLL general clock output) = f(VCO clock) / PLLP
    */
    volatile static struct PLL_mulls config;
    volatile float PLL_float;
    int PLL_int;
    uint8_t i;
    for (i = PLLP_2_mull; i < PLLP_4_mull; i++)
    {
        config.PLLP = i;
        for (config.PLLN = 50; config.PLLN < 433; config.PLLN++)
        {
            for (config.PLLM = 2; config.PLLM < 64; config.PLLM++)
            {
                PLL_float = config.PLLN / config.PLLM;
                if ((float)(PLL_float) == ((float)(int)(PLL_float)))
                    if (((((HSE_Mhz) * (int)(PLL_float)) / ((i + 1) * 2)) == needed_Mhz))
                    {
                        return &config;
                    }
            }
        }
    }
    config.PLLN = -1;
    return &config;
}