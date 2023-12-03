#include "ADC_Stm32F429Zi.h"

void giveClockADC(ADC_TypeDef *ADC_using);

void initADC(ADC_TypeDef *ADC_using, enum Resolution_ADC resolution, enum Type_of_channel channel_type,
             enum Discontinuous_mode discontinuous_mode, uint8_t num_of_discontinuos_channels,
             enum Automativ_conversion auto_conv, enum Scan_mode scan_mode, uint8_t interrupts)
{
    giveClockADC(ADC_using);
    ADC_using->CR1 = (resolution << 24);
    if (channel_type == Regular_channel)
    {
        ADC_using->CR1 |= (num_of_discontinuos_channels << 13);
    }
    ADC_using->CR1 |= (discontinuous_mode << (12 + channel_type)) | (auto_conv << 10) |
                      (scan_mode << 8) | (interrupts << 5);
    ADC_using->CR2 = 1;
}

void giveClockADC(ADC_TypeDef *ADC_using)
{
    switch ((unsigned int)ADC_using)
    {
    case ((unsigned int)ADC1):
        RCC->APB2ENR |= (1 << 8);
        break;
    case ((unsigned int)ADC2):
        RCC->APB2ENR |= (1 << 9);
        break;
    case ((unsigned int)ADC3):
        RCC->APB2ENR |= (1 << 10);
        break;
    }
}

void initADCChannel(ADC_TypeDef *ADC_using, uint8_t num_of_channel, uint8_t sample_time, enum Type_of_channel type, uint16_t offset)
{
    *(unsigned int *)((unsigned int)(&ADC_using->SMPR2) - ((num_of_channel / 10) * 4)) = sample_time << ((num_of_channel % 10) * 3);
}

void initChannelsMassiv(ADC_TypeDef *ADC_using, uint8_t *num_of_channel, uint8_t num_of_conv, enum Type_of_channel type)
{
    if (type == Regular_channel)

    {
        for (uint8_t i = 0; i < num_of_conv; i++)
            *(unsigned int *)((unsigned int)(&ADC_using->SQR3) - ((i / 6) * 4)) |= num_of_channel[i] << (5 * (i % 6));
        ADC_using->SQR1 |= ((num_of_conv - 1) << 20);
    }
    else
    {
        for (uint8_t i = 0; i < num_of_conv; i++)
            ADC_using->JSQR |= num_of_channel[i] << (5 * i);
        ADC_using->JSQR |= ((num_of_conv - 1) << 20);
    }
}