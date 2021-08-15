
#define RCGCGPIO (*((unsigned int *) 0x400FE608U)) // Clock gating control

#define    GPIOF_BASE   0x40025000U
#define    GPIOF_DIR    (*((volatile unsigned long*) (GPIOF_BASE + 0x400U)))
#define    GPIOF_DEN    (*((volatile unsigned long*) (GPIOF_BASE + 0x51CU)))
#define    GPIOF_DATA   (*((volatile unsigned long*) (GPIOF_BASE + 0x3FCU)))

void delay();

int main()
{
    // Set RCGCGPIO with 0x20U - Give Port F a clock
    RCGCGPIO = 0x20U;
    
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
