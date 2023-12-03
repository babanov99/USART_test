#ifndef GPIO_STM429
#define GPIO_STM429
#include "stm32f429xx.h"
#include <stdint.h>
enum MODE_PIN
{
    MODE_Input = 0b00,
    MODE_Output = 0b01,
    MODE_Alt = 0b10,
    MODE_Analog = 0b11
};

enum TYPE_PIN
{
    TYPE_PP = 0,
    TYPE_OD = 1
};

enum SPEED_PIN
{
    SPEED_Low = 0b00,
    SPEED_Medium = 0b01,
    SPEED_High = 0b10,
    SPEED_VHight = 0b11
};
enum PULL_PIN
{
    PULL_NO = 0b00,
    PULL_UP = 0b01,
    PULL_DOWN = 0b10
};
void initGPIO(GPIO_TypeDef *GPIO_using, uint8_t pin_num, enum MODE_PIN mode,
              enum TYPE_PIN type, enum SPEED_PIN speed, enum PULL_PIN pull, uint8_t type_alt);
#endif