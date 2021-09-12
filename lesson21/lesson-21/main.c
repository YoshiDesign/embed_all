#include <stdint.h>
#include "bsp.h"

// Blinky 1 stack allocator. Each element corresponds to 1 32b register
uint32_t stack_blink1[40];
uint32_t *sp_blink1 = &stack_blink1[40];

/* background code: sequential with blocking version */
int main_blink1() {
    
    while (1) {
        BSP_ledGreenOn();
        BSP_delay(BSP_TICKS_PER_SEC / 4U);
        BSP_ledGreenOff();
        BSP_delay(BSP_TICKS_PER_SEC * 3U / 4U);
    }

    //return 0;
}

// Blinky 2 stack allocator
uint32_t stack_blink2[40];
uint32_t *sp_blink2 = &stack_blink2[40];

/* background code: sequential with blocking version */
int main_blink2() {
    
    while (1) {
        BSP_ledBlueOn();
        BSP_delay(BSP_TICKS_PER_SEC / 2U);
        BSP_ledBlueOff();
        BSP_delay(BSP_TICKS_PER_SEC / 3U);
    }

    //return 0;
}

int main() {

    BSP_init();
    
    // REMEMBER: If we're decrementing sp_blink, we're now 32 bits away from where we were last pointing!
    // Since we're beginning at sp_blink[40], we decrement first!!
    
    // Pre-fill each fabricated stack with a Cortex ISR stack frame
    // THE ALIGNER is not necessary to account for because we 8-byte aligned from the beginning- i.e. [40]
    *(--sp_blink1) = (1U << 24);    /* xPSR - Setting the 24th bit for historical purposes */
    *(--sp_blink1) = (uint32_t)&main_blink1;    // Pointer to function address
    *(--sp_blink1) = 0x0000000EU;   /* LR */
    *(--sp_blink1) = 0x0000000CU;   /* R12 */
    *(--sp_blink1) = 0x00000003U;   /* R3 */
    *(--sp_blink1) = 0x00000002U;   /* R2 */
    *(--sp_blink1) = 0x00000001U;   /* R1 */
    *(--sp_blink1) = 0x00000000U;   /* R0 */
    
    *(--sp_blink2) = (1U << 24);   /* xPSR */
    *(--sp_blink2) = (uint32_t)&main_blink2;    // Pointer to function address
    *(--sp_blink2) = 0x0000000EU;   /* LR */
    *(--sp_blink2) = 0x0000000CU;   /* R12 */
    *(--sp_blink2) = 0x00000003U;   /* R3 */
    *(--sp_blink2) = 0x00000002U;   /* R2 */
    *(--sp_blink2) = 0x00000001U;   /* R1 */
    *(--sp_blink2) = 0x00000000U;   /* R0 */

    while(1){}

}