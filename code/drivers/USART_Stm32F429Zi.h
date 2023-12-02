#ifndef USART_STM32F42
#define USART_STM32F4
#include <stdint.h>
#include <stm32f429xx.h>
#include <math.h>
#define PE_interrupt (0b1 << 8)
#define TXE_interrupt (0b1 << 7)
#define TXC_interrupt (0b1 << 6)
#define RXNE_interrupt (0b1 << 5)
#define IDLE_interrupt (0b1 << DMA_SxNDT_4)

enum what_is_it_enum
{
    ITS_UART = 0,
    ITS_USART = 1
};

enum length_enum
{
    length_8 = 0,
    length_9 = 1
};

enum parity_enum
{
    lengthparity_disable = 0,
    parity_enable_even = 0b10,
    parity_enable_odd = 0b11
};

struct BAUD_RATE
{
    uint8_t OVER8_data;
    uint16_t mantisa;
    uint8_t freq;
    float need_BOD;
};

enum STOP_BITS
{
    Stop_bit_1 = 0b00,
    Stop_bit_0 = 0b01,
    Stop_bit_2 = 0b10,
    Stop_bi_15 = 0b11
};
enum error_USART
{
    NICE_USART = 0,
    ERROR_NOT_USART = 1,
    ERROR_CANT_CALCULATE_RIGHT = 2
};

enum error_USART initUSART(USART_TypeDef *USING_USART, enum what_is_it_enum what_is_it,
                           enum length_enum length, enum parity_enum parity, enum STOP_BITS stop_bit_length,
                           uint8_t interrupts, uint32_t CPU_freq, uint32_t neededBOD);
#endif