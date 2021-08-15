#include "heads.h"

void delay(int duration)
{
    volatile int counter = 0;
    while(counter < duration) {
        ++counter;
    }

    return;

}