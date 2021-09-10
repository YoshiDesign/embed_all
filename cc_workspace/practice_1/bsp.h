#ifndef __BSP_H__
#define __BSP_H__


#define SYS_CLOCK_HZ 16000000U  

#define aprint(expr) printf(#expr ": %x\n", expr)
#define LED_RED     (1U << 1)   // 0x02
#define LED_BLUE    (1U << 2)   // 0x04
#define LED_GREEN   (1U << 3)   // 0x08


void delay(int duration);

#endif // __BSP_H__