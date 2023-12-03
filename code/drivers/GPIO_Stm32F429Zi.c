#include "GPIO_Stm32F429Zi.h"

void giveClockToPort(GPIO_TypeDef *GPIO_using);
/// @brief Инициализация пина
/// @param GPIO_using Адрес порта
/// @param pin_num Номер пина
/// @param mode Вид пина
/// @param type Тип пина
/// @param speed Скорость пина
/// @param pull Подтяжка
/// @param type_alt Тип альтернативной функции
void initGPIO(GPIO_TypeDef *GPIO_using, uint8_t pin_num, enum MODE_PIN mode, enum TYPE_PIN type, enum SPEED_PIN speed, enum PULL_PIN pull, uint8_t type_alt)
{
    giveClockToPort(GPIO_using);
    GPIO_using->MODER |= (mode << (2 * pin_num));
    GPIO_using->OTYPER |= (type << (pin_num));
    GPIO_using->OSPEEDR |= (speed << (2 * pin_num));
    GPIO_using->PUPDR |= (pull << (2 * pin_num));
    GPIO_using->AFR[pin_num / 8] |= type_alt << ((pin_num % 8) * 3);
}
/// @brief Подать тактирование на порт
/// @param GPIO_using Адрес порта
void giveClockToPort(GPIO_TypeDef *GPIO_using)
{
    switch ((unsigned int)GPIO_using)
    {
    case ((unsigned int)GPIOA):
        RCC->AHB1ENR |= (1 << 0);
        break;
    case ((unsigned int)GPIOB):
        RCC->AHB1ENR |= (1 << 1);
        break;
    case ((unsigned int)GPIOC):
        RCC->AHB1ENR |= (1 << 2);
        break;
    case ((unsigned int)GPIOD):
        RCC->AHB1ENR |= (1 << 3);
        break;
    case ((unsigned int)GPIOE):
        RCC->AHB1ENR |= (1 << 4);
        break;
    case ((unsigned int)GPIOF):
        RCC->AHB1ENR |= (1 << 5);
        break;
    case ((unsigned int)GPIOG):
        RCC->AHB1ENR |= (1 << 6);
        break;
    case ((unsigned int)GPIOH):
        RCC->AHB1ENR |= (1 << 7);
        break;
    case ((unsigned int)GPIOI):
        RCC->AHB1ENR |= (1 << 8);
        break;
    case ((unsigned int)GPIOJ):
        RCC->AHB1ENR |= (1 << 9);
        break;
    case ((unsigned int)GPIOK):
        RCC->AHB1ENR |= (1 << 10);
        break;
    default:
        break;
    }
}