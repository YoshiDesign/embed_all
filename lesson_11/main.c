#include <stdio.h>
#include <stdint.h>

#include "heads.h"
#include "Include/tm4c_cmsis.h"

#define aprint(expr) printf(#expr ": %x\n", expr)
#define LED_RED     (1U << 1)   // 0x02
#define LED_BLUE    (1U << 2)   // 0x04
#define LED_GREEN   (1U << 3)   // 0x08

// Best practice struct def if struct doesn't reference itself
typedef struct {
    uint8_t y;
    uint16_t x;
} Point; 

typedef struct {
    Point top_left;
    Point bottom_right;
} Window;

typedef struct {
    Point corners[3];
} Triangle;

Window w;
Triangle t;

Point p1, p2;

// Note that tag names occupy a different namespace in C, so they can have the same name as the type
// typedef struct Point Point;
// Point p1, p2;

int main()
{

    Point *ppt;
    Window *wpt;
    
    ppt = &p1;
    wpt = &w;
    
    // Parentheses are needed here bc the precedence of the dot operator is higher than
    // that of the dereference operator
    (*ppt).x = 1U;
    (*wpt).bottom_right = p1;
    // ALTERNATIVELY we are given the arrow operator
    ppt->x = 3U;
    wpt->bottom_right = p2;
    
    p1.x = sizeof(Point);
    p1.y = 0xAAU;
    
    w.top_left.x = 1U;
    w.bottom_right.y = 2U;
    
    t.corners[0].x = 1U;
    
    // Set RCGCGPIO with 0x20U - Give Port F a clock
    SYSCTL->RCGC2 |= (1U << 5);
    
    
    // Enable AHB for GPIOF - All 3 are identical
    // (*((volatile unsigned long*)0x400FE06C)) = 0x20;
    // SYSCTL_GPIOHBCTL_R = 0x20;
    SYSCTL->GPIOHSCTL |= (1U << 5);
    
    // Set memory R1 R2 and R3 to 1 in GPIODIR - Direction Output
    GPIOF_HS->DIR |= (LED_RED | LED_BLUE | LED_GREEN);
    
    // Set memory R1 R2 and R3 to 1 in GPIODEN - Digital Enable
    GPIOF_HS->DEN |= (LED_RED | LED_BLUE | LED_GREEN);
    
    // Turn on the blue led component
    // GPIO_PORTF_DATA_R |= LED_BLUE;           // Idiomatic
    GPIOF_HS->DATA_Bits[LED_BLUE] = LED_BLUE;   // Atomic
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
        GPIOF_HS->DATA_Bits[LED_RED] = LED_RED;
       
        delay(300000);
        
        // GPIO_PORTF_DATA_R &= ~LED_RED;       // Idiomatic clear bit
        GPIOF_HS->DATA_Bits[LED_RED] = 0;    // Atomic clear bit
        delay(300000);
    }
    return 0;
}

