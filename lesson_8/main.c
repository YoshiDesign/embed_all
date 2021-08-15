#include <stdio.h>
#include "heads.h"
#include "lm4f120h5qr.h"
#define aprint(expr) printf(#expr ": %x\n", expr)
#define LED_RED     (1U << 1)   // 0x02
#define LED_BLUE    (1U << 2)   // 0x04
#define LED_GREEN   (1U << 3)   // 0x08


unsigned fact(unsigned n);

int main()
{
    
    unsigned volatile x;
    
    x = fact(0U);
    x = 2U + 3U*fact(1U);
    
    // Casting return to void to explicitly mean that we don't care about the return value
    (void)fact(5U);
    
    unsigned int a = 0xFA5A5A5A;
    unsigned int b = 0xDEADBEEF;
    unsigned int c;
    
    c = a | b;
    c = a & b;
    c = a ^ b;
    c = ~b;
    c = b >> 1;
    c = b << 3;
    
    // Set RCGCGPIO with 0x20U - Give Port F a clock
    SYSCTL_RCGCGPIO_R |= (1U << 5);
    
    // Enable AHB for GPIOF - All 3 are identical
    // (*((volatile unsigned long*)0x400FE06C)) = 0x20;
    // SYSCTL_GPIOHBCTL_R = 0x20;
    SYSCTL_GPIOHBCTL_R |= (1U << 5);
    
    // Set memory R1 R2 and R3 to 1 in GPIODIR - Direction Output
    GPIO_PORTF_AHB_DIR_R |= (LED_RED | LED_BLUE | LED_GREEN);
    
    // Set memory R1 R2 and R3 to 1 in GPIODEN - Digital Enable
    GPIO_PORTF_AHB_DEN_R |= (LED_RED | LED_BLUE | LED_GREEN);
    
    // Turn on the blue led component
    // GPIO_PORTF_DATA_R |= LED_BLUE;   // Idiomatic
    GPIO_PORTF_AHB_DATA_BITS_R[LED_BLUE] = LED_BLUE; // Atomic
    while (1){
        
        /**
         * Atomic operations are interrupt safe - they will not discard values from interrupts
         * because they only use one MOV instruction, instead of 3 (the read-modify-write idiom)
         */
        
        // (IDIOMATIC) Toggle value 0010 (Red LED) from GPIODATA -- Follows the Read-Modify-Write idiom
        // GPIO_PORTF_DATA_R |= LED_RED;   // Idiomatic to set bit
          
        /**
         * (ATOMIC - Brute force approach) We call this "casting a synthesized address to a pointer". 
         * Left shift 2 corresponds to the fact that we ignore the first 2 bits of our GPIO_PORTF register
         * (our processor requires that all addresses are divisible by 4)
         *///*((unsigned long volatile *)(0x40025000 + (LED_RED << 2))) = LED_RED;
        
        
        // (ATOMIC) Access the LED_RED 32 BIT ADDRESS and set to 0x02 via pointer
        // *(GPIO_PORTF_DATA_BITS_R + LED_RED) = LED_RED;
        
        // (ATOMIC) Access the LED_RED 32 BIT ADDRESS as a pointer from array indexing and write LED_RED
        GPIO_PORTF_AHB_DATA_BITS_R[LED_RED] = LED_RED;
       
        delay(1000000);
        
        // GPIO_PORTF_DATA_R &= ~LED_RED;       // Idiomatic clear bit
        GPIO_PORTF_AHB_DATA_BITS_R[LED_RED] = 0;    // Atomic clear bit
        delay(500000);
    }
    return 0;
}


unsigned fact(unsigned n)
{
    // Recursive factorial
    // 0! = 1
    // n! = n*(n-1)! for n > 0
    
    if (n == 0U) {
        return 1U;
    }
    else {
        return n * fact(n -1U);
    }
    
}