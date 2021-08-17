#include <stdio.h>
#include <stdint.h>

#include "heads.h"
#include "Include/tm4c_cmsis.h"

#define SYS_CLOCK_HZ 16000000U  

#define aprint(expr) printf(#expr ": %x\n", expr)
#define LED_RED     (1U << 1)   // 0x02
#define LED_BLUE    (1U << 2)   // 0x04
#define LED_GREEN   (1U << 3)   // 0x08

int main()
{


 
    SYSCTL->RCGC2 |= (1U << 5);
    
    SYSCTL->GPIOHSCTL |= (1U << 5);
    
    // Set memory R1 R2 and R3 to 1 in GPIODIR - Direction Output
    GPIOF_HS->DIR |= (LED_RED | LED_BLUE | LED_GREEN);
    
    // Set memory R1 R2 and R3 to 1 in GPIODEN - Digital Enable
    GPIOF_HS->DEN |= (LED_RED | LED_BLUE | LED_GREEN);
    
    // Configure and enable SysTick interrupt to do its thing every .5s
    // Using the SysTick Registers (Interrupt peripheral)
    // SysTick's counter has a dynamic range which we need to be sure not to overflow. It is 24bits
    // So when we cut our clock frequency in half - 1, we need to make sure the value (7,999,999) fits. It does!
    SysTick->LOAD = SYS_CLOCK_HZ/2U - 1U;
    SysTick->VAL = 0U; // (This register is clear-on-write) Clears the bit no matter what you write to it
    SysTick->CTRL = (1U << 2) | (1U << 1) | 1U;
    
    // Turn on the blue led component
    // GPIO_PORTF_DATA_R |= LED_BLUE;           // Idiomatic
    GPIOF_HS->DATA_Bits[LED_BLUE] = LED_BLUE;   // Atomic
    while (1){

        // (ATOMIC) Access the LED_RED 32 BIT ADDRESS as a pointer from array indexing and write LED_RED
        GPIOF_HS->DATA_Bits[LED_RED] ^= LED_RED;
       
        delay(300000);

    }
    return 0;
}

