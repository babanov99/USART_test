#ifndef ADC_STM32
#define ADC_STM32
#include "stm32f429xx.h"
enum Resolution_ADC
{
    Resolution_12_bit = 0b00, // (15 ADCCLK cycles)
    Resolution_10_bit = 0b01, // (13 ADCCLK cycles)
    Resolution_8_bit = 0b10,  //(11 ADCCLK cycles)
    Resolution_6_bit = 0b11   //(9 ADCCLK cycles)
};

enum Type_of_channel
{
    Injected_channel = 0,
    Regular_channel = 1
};

enum Discontinuous_mode
{
    mode_disable = 0,
    mode_enable = 1
};

enum Automativ_conversion
{
    conversion_disable = 0,
    conversion_enable = 1
};

enum Scan_mode
{
    scan_mode_disable = 0,
    scan_mode_enable = 1
};
void initADC(ADC_TypeDef *ADC_using, enum Resolution_ADC resolution, enum Type_of_channel channel_type,
             enum Discontinuous_mode discontinuous_mode, uint8_t num_of_discontinuos_channels,
             enum Automativ_conversion auto_conv, enum Scan_mode scan_mode, uint8_t interrupts);
void initADCChannel(ADC_TypeDef *ADC_using, uint8_t num_of_channel, uint8_t sample_time, enum Type_of_channel type, uint16_t offset);
void initChannelsMassiv(ADC_TypeDef *ADC_using, uint8_t *num_of_channel, uint8_t num_of_conv, enum Type_of_channel type);
#endif