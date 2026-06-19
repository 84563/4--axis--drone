#ifndef __LED_H__
#define __LED_H__


#include "main.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
}LED_Struct;



/**
 * @brief LED开灯
 * 
 */
void led_on(LED_Struct *led);

/**
 * @brief LED关灯
 * 
 */
void led_off(LED_Struct *led);

/**
 * @brief LED翻转
 * 
 */
void led_toggle(LED_Struct *led);


#endif
