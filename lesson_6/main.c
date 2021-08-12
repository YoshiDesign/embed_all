#include "include/lm4f120h5qr.h"

void delay();

int main()
{
    // Set RCGCGPIO with 0x20U - Give Port F a clock
    SYSCTL_RCGCGPIO_R = 0x20U;
    
    // Set memory R1 R2 and R3 to 1 in GPIODIR - Direction Output
    GPIOF_DIR = 0x0EU;
    
    // Set memory R1 R2 and R3 to 1 in GPIODEN - Digital Enable
    GPIOF_DEN = 0x0EU;
    
    while (1){
        // Toggle value 0010 (Red LED) from GPIODATA
        GPIOF_DATA = 0x05;
        delay();
        GPIOF_DATA = 0x00;
        delay();
    }
    return 0;
}

void delay()
{
    int counter = 0;
    while(counter < 600000) {
        ++counter;
    }
    
    return;

}
