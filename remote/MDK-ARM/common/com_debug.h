#ifndef __COM_DEBUG_H__
#define __COM_DEBUG_H__


#include "usart.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

//日志输出打印在cpu上非常占用资源=>通过比特率计算 打印10字节耗时1ms 非常影响飞机飞行
//飞机飞行需要关闭打印功能
//设置一个日志输出开关
#define DEBUG_LOG_ENABLE 1

#ifdef DEBUG_LOG_ENABLE

//只打印文件名不要路径
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


//使用宏定义的方式能实现打印日志之前，先添加文件名和行号信息
#define debug_printf( format ,...) printf("[%s : %d]" format, __FILENAME__ , __LINE__ ,##__VA_ARGS__)

#else

//如果没有开启日志输出打印
#define debug_printf( format ,...)  

#endif

#endif
