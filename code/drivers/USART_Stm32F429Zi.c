#include "USART_Stm32F429Zi.h"

struct BAUD_RATE *calculateBaudRate(uint32_t CPU_freq, uint32_t neededBOD);

// TODO настройки USART

/// @brief Инициализация USART
/// @note Не до конца настройки
/// @param USING_USART ссылка на USART
/// @param what_is_it_enum USART или UART
/// @param length_enum длина посылки
/// @param parity_enum честность
/// @param STOP_BITS кол-во стоп бит
/// @param interrupts какие прерывания использовать
/// @param CPU_freq частота процессора !НЕ В МЕГОГЕРЦАХ! (хотя мб потом переделаю)
/// @param neededBOD требуемые БОДы
/// @return Состояние о USART
enum error_USART initUSART(USART_TypeDef *USING_USART, enum what_is_it_enum what_is_it,
                           enum length_enum length, enum parity_enum parity, enum STOP_BITS stop_bit_length,
                           uint8_t interrupts, uint32_t CPU_freq, uint32_t neededBOD)
{
    // честно - лень делать настройки для каждого случая, но по возможности позже сделаю, тут только база
    if (USING_USART == USART1)
    {
        RCC->APB2ENR |= (1 << 4);
    }
    if (USING_USART == USART2)
    {
        RCC->APB1ENR |= (1 << 17);
    }
    if (USING_USART == USART3)
    {
        RCC->APB1ENR |= (1 << 18);
    }
    if (USING_USART == UART4)
    {
        if (what_is_it == ITS_USART)
            return ERROR_NOT_USART;
        RCC->APB1ENR |= (1 << 19);
    }
    if (USING_USART == UART5)
    {
        if (what_is_it == ITS_USART)
            return ERROR_NOT_USART;
        RCC->APB1ENR |= (1 << 20);
    }
    if (USING_USART == USART6)
    {
        RCC->APB2ENR |= (1 << 5);
    }
    struct BAUD_RATE *data_baud = calculateBaudRate(CPU_freq, neededBOD);
    if ((data_baud->mantisa == 0) && (data_baud->freq == 0))
        return ERROR_CANT_CALCULATE_RIGHT;
    USING_USART->BRR = (data_baud->mantisa << 4) | (data_baud->freq);
    USING_USART->CR1 |= (data_baud->OVER8_data << 15) | (1 << 13) | (length << 12) | (parity << 10) |
                        (interrupts << 4) | (0b11 << 2);
    if (what_is_it == ITS_USART)
        USING_USART->CR2 |= (1 << 11);
    USING_USART->CR2 |= stop_bit_length << 12;
    return NICE_USART;
}

struct BAUD_RATE *calculateBaudRate(uint32_t CPU_freq, uint32_t neededBOD)
{
    /*
    Tx/Rx baud=fCK/(8*(2-OVER8)*USARTDIV
    */
    static struct BAUD_RATE settings[2];
    float USART_div[2];
    float mantisa_temp[2];
    uint8_t OVER8_data;
    for (OVER8_data = 0; OVER8_data < 2; OVER8_data++)
    {
        USART_div[OVER8_data] = CPU_freq / (8 * (2 - OVER8_data) * neededBOD);
        settings[OVER8_data].mantisa = USART_div[OVER8_data];
        mantisa_temp[0] = (uint8_t)((USART_div[OVER8_data] - (uint8_t)USART_div[OVER8_data]) * 16);
        mantisa_temp[1] = (uint8_t)((USART_div[OVER8_data] - (uint8_t)USART_div[OVER8_data]) * 16) + 1;
        if ((fabs(mantisa_temp[0] / 16) - (USART_div[OVER8_data] - (uint8_t)USART_div[OVER8_data])) >
            fabs((mantisa_temp[1] / 16) - (USART_div[OVER8_data] - (uint8_t)USART_div[OVER8_data])))
            settings[OVER8_data].freq = mantisa_temp[0];
        else
            settings[OVER8_data].freq = mantisa_temp[1];
        if (settings[OVER8_data].freq > 0b1111)
        {
            settings[OVER8_data].mantisa++;
        }
        settings[OVER8_data].freq &= 0b1111;
        settings[OVER8_data].OVER8_data = OVER8_data;
        settings[OVER8_data].need_BOD = 0;
        if ((settings[OVER8_data].mantisa == 0) && ((settings[OVER8_data].freq / 16) == 0))
            continue;
        settings[OVER8_data].need_BOD = CPU_freq / (8 * (2 - OVER8_data) * (float)(settings[OVER8_data].mantisa + (settings[OVER8_data].freq / 16)));
    }
    if ((fabs(settings[0].need_BOD - neededBOD) > fabs(settings[1].need_BOD - neededBOD)))
        return &settings[1];
    return &settings[0];
}