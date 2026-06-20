#include "com_tool.h"



/**
 * @brief 限制值在指定范围
 * 
 * @param value 值
 * @param min 最小值
 * @param max 最大值
 * @return uint16_t 限制后的值
 */
uint16_t com_limit(uint16_t value, uint16_t min, uint16_t max)
{
    uint16_t result = value;
    if (value < min)
    {
        result = min;
    }
    else if (value > max)
    {
        result = max;
    }
    return result;
}
