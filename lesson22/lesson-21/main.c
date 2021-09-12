#include <stdint.h>
#include "skios.h"
#include "bsp.h"

/**
Important concepts.
- Context switching must occur during the return from an ISR, such as SysTick
- ISR's can be pure C function with the ARM Cortex-M instruction set.
- Context switching 

*/

// Blinky 1 stack allocator and it's thread handler. 
// Each element of the stack corresponds to 1 32b register
uint32_t stack_blink1[40];
OSThread sp_blink1;
// Blinky 2 stack allocator
uint32_t stack_blink2[40];
OSThread sp_blink2;

void main_blink1() {
    
    while (1) {
        BSP_ledGreenOn();
        BSP_delay(BSP_TICKS_PER_SEC / 4U);
        BSP_ledGreenOff();
        BSP_delay(BSP_TICKS_PER_SEC * 3U / 4U);
    }

}

void main_blink2() {
    
    while (1) {
        BSP_ledBlueOn();
        BSP_delay(BSP_TICKS_PER_SEC / 2U);
        BSP_ledBlueOff();
        BSP_delay(BSP_TICKS_PER_SEC / 3U);
    }

}

int main() {

    BSP_init();
    OS_init();
    
    // Fabricate Cortex-M ISR stack frame for blink1 and 2
    OSThread_start(&sp_blink1, &main_blink1, stack_blink1, sizeof(stack_blink1));
    OSThread_start(&sp_blink2, &main_blink2, stack_blink2, sizeof(stack_blink2));


    while(1){}

}
