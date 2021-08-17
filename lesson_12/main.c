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

Window w = {

    {10U, 0x105U},
    {10U, 0x105CU}
    
};

Triangle t;

Point p1, p2; 

// Note that tag names occupy a different namespace in C, so they can have the same name as the type
// typedef struct Point Point;
// Point p1, p2;

int main()
{

    Window r = {
        {12U, 12U},
        {12U, 12U}
    };
    
    Window *p = &r;
    Point x, y, z;
    
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

        // (ATOMIC) Access the LED_RED 32 BIT ADDRESS as a pointer from array indexing and write LED_RED
        GPIOF_HS->DATA_Bits[LED_RED] ^= LED_RED;
       
        delay(300000);

    }
    return 0;
}

