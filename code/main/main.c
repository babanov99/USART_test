#include "main.h"

int main(void)
{
	volatile enum error_PLL error = initCorePLL(HSE, 8, 48);
	if (error == NICE)
	{
		initGPIO(GPIOB, 6, MODE_Alt, TYPE_PP, SPEED_VHight, PULL_NO, 7);
		initGPIO(GPIOB, 7, MODE_Alt, TYPE_PP, SPEED_VHight, PULL_NO, 7);
		GPIOB->AFR[0] |= (0b111 << 24);
		volatile enum error_USART errors = initUSART(USART1, ITS_UART, length_8, parity_enable_even, Stop_bit_2, 0, 48000000, 9400);
		USART1->DR = 0b11;
		while (1)
		{
		}
	}
	else
		while (1)
		{
			__NOP();
		}
	return 0;
}
