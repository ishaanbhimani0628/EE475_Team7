#include <msp430.h> 
#include <stdint.h>
#include "adxl362.h"
#include "uart.h"


/**
 * main.c
 */

volatile  int16_t x_data = 0;



int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL1 = CALBC1_1MHZ;      // Set range
    DCOCTL = CALDCO_1MHZ;      // SMCLK = DCO = 1MHz

    uart_init();
    ADXL362_configSetupBegin();
    __enable_interrupt();

//    int16_t y_data = 0;
//    int16_t z_data = 1;
    int i;
    while(1) {
        x_data = ADXL362_readXYZData();
        char buf[16];
        for (i = 0; i < 16; i++) {
            buf[i] = *(((char*) &x_data) + i);
        }

        uart_puts(buf);

    }
}

