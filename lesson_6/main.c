#include "include/lm4f120h5qr.h"
#define aprint(expr) printf(#expr ": %x\n", expr)
#define LED_RED     (1U << 1)   // 0x02
#define LED_BLUE    (1U << 2)   // 0x04
#define LED_GREEN   (1U << 3)   // 0x08
void delay();

int main()
{
    unsigned int a = 0xFA5A5A5A;
    unsigned int b = 0xDEADBEEF;
    unsigned int c;
    
    c = a | b;
    c = a & b;
    c = a ^ b;
    c = ~b;
    c = b >> 1;
    c = b << 3;
    
    int x = 5;
    int z = x << 3;
    
    // Set RCGCGPIO with 0x20U - Give Port F a clock
    SYSCTL_RCGCGPIO_R |= (1U << 5);
    
    // Set memory R1 R2 and R3 to 1 in GPIODIR - Direction Output
    GPIO_PORTF_DIR_R |= (LED_RED | LED_BLUE | LED_GREEN);
    
    // Set memory R1 R2 and R3 to 1 in GPIODEN - Digital Enable
    GPIO_PORTF_DEN_R |= (LED_RED | LED_BLUE | LED_GREEN);
    
    // Turn on the blue led component
    GPIO_PORTF_DATA_R |= LED_BLUE;
    while (1){
        // Toggle value 0010 (Red LED) from GPIODATA
        GPIO_PORTF_DATA_R |= LED_RED;   // Idiomatic to set bit
        delay();
        
        GPIO_PORTF_DATA_R &= ~LED_RED;  // Idiomatic clear bit
        delay();
    }
    return 0;
}

void delay()
{
    int counter = 0;
    while(counter < 1000000) {
        ++counter;
    }

    return;

}
