#include "main.h"
#include <stdio.h>
#include <EventRecorder.h>
#define CHANNEL_ADC 1
int main(void)
{
	uint8_t data[] = {CHANNEL_ADC};
	RCC->CFGR |= (0b100 << 27);
	RCC->CFGR |= (0b00 << 30);
	volatile enum error_PLL error = initCorePLL(HSE, 8, 16);
	if (error == NICE)
	{
		EventRecorderInitialize(EventRecordAll, 1);
		initGPIO(GPIOB, 6, MODE_Alt, TYPE_PP, SPEED_VHight, PULL_NO, 7);
		initGPIO(GPIOB, 7, MODE_Alt, TYPE_PP, SPEED_VHight, PULL_NO, 7);
		volatile enum error_USART errors = initUSART(USART1, ITS_UART, length_8, parity_enable_even, Stop_bit_2, 0, 80000000, 9400);
		initADC(ADC1, Resolution_12_bit, Regular_channel, mode_disable, 0, conversion_disable, scan_mode_enable, 0);
		initADCChannel(ADC1, CHANNEL_ADC, 0b111, Regular_channel, 0);
		initChannelsMassiv(ADC1, data, 1, Regular_channel);
		ADC->CCR |= (1 << 23) | (0b11 << 16);
		initGPIO(GPIOA, 1, MODE_Analog, TYPE_PP, SPEED_VHight, PULL_NO, 0);
		printf("Initialization_complite\n");
	}
	while (1)
	{
		ADC1->CR2 |= (1 << 30);
		while (!(ADC1->SR & (1 << 1)))
			;
		printf("%x", ADC1->DR);
	}
	return 0;
}
