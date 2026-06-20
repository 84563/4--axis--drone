#include "key.h"


/**
 * @brief 获取按键是否按下 0按下 1抬起
 * @return key_type KEY_NONE:没有按键按下
 */
key_type key_get(void)
{
    if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
        {
            //等待按键释放
            while (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_UP;
        }
    }
    else if (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port,KEY_DOWN_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
        {   
            //等待按键释放
            while (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_DOWN;
        }
    }
     
    else if (HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port,KEY_LEFT_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
        {   
            //等待按键释放
            while (HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_LEFT;
        }
    }  
    else if (HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port,KEY_RIGHT_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
        {   
            //等待按键释放
            while (HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_RIGHT;
        }
    }
    //左上按键----关机
     else if (HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port,KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
        {   
            //等待按键释放
            while (HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            return KEY_LEFT_X;
        }
    }  
    //右上按键 短按--定高   长按---校准摇杆
    else if (HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port,KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
    {
        //计时
        TickType_t count1 = xTaskGetTickCount();
        vTaskDelay(5);
        if(HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
        {   
            //等待按键释放
            while (HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            }
            TickType_t count2 = xTaskGetTickCount();
            if(count2 - count1 > 1000)
            {
                return KEY_RIGHT_X_LONG;
            }
            else
            {
                return KEY_RIGHT_X;
            }
        }
    }
    //没有按键按下
    return KEY_NONE;
}





