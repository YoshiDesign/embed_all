#include <stdio.h>
#include <stdint.h>

#include "heads.h"
#include "lm4f120h5qr.h"

#define aprint(expr) printf(#expr ": %x\n", expr)
#define LED_RED     (1U << 1)   // 0x02
#define LED_BLUE    (1U << 2)   // 0x04
#define LED_GREEN   (1U << 3)   // 0x08

uint8_t u8a, u8b;
uint16_t u16c, u16d;
uint32_t u32e, u32f;

int8_t s8;
int16_t s16;
int32_t s32;

unsigned fact(unsigned n);
int *swap(int *x, int *y);
int *swap(int *x, int*y)
{
    // Static tells the compiler to allocate this variable outside of the callstack, so that it can persist in memory after the function returns.
    static int tmp[2];
    tmp[0] = *x;
    tmp[1] = *y;
    
    *x = tmp[1];
    *y = tmp[0];
    
    return tmp;
    
}
int main()
{
    
    u8a  = sizeof(u8a);
    u16c = sizeof(uint16_t);
    u32e = sizeof(uint32_t);
    
    u8a = 0xa1U;
    u16c = 0xc1c2U;
    u32e = 0xe1e2e3e4U;
    
    u8b  = u8a;
    u16d = u16c;
    u32f = u32e;
    
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
        
        int x = 1000000;
        int y = 1000000/2;
        int *p = swap(&x, &y);
        
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
       
        delay(p[0]);
        
        // GPIO_PORTF_DATA_R &= ~LED_RED;       // Idiomatic clear bit
        GPIO_PORTF_AHB_DATA_BITS_R[LED_RED] = 0;    // Atomic clear bit
        delay(p[1]);
    }
    return 0;
}


unsigned fact(unsigned n)
{
    // Recursive factorial
    // 0! = 1
    // n! = n*(n-1)! for n > 0
    
    unsigned foo[100];
    foo[n] = n;
    
    if (n == 0U) {
        return 1U;
    }
    else {
        return foo[n] * fact(n -1U);
    }
    
}