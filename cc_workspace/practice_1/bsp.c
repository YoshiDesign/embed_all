#include "Include/TM4C123GH6PM.h"
#include "bsp.h"

__attribute__((naked)) void assert_failed (char const *file, int line) {
    /*TBD Damage Control*/
    NVIC_SystemReset();
}

void SysTick_Handler(void) {
    GPIOF_AHB->DATA_Bits[LED_BLUE] ^= LED_BLUE;
}
