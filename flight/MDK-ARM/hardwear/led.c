#include "led.h"


void led_on(LED_Struct *led)
{
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);

}
void led_off(LED_Struct *led)
{
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}
void led_toggle(LED_Struct *led)
{
    HAL_GPIO_TogglePin(led->port, led->pin);
}
