/******************************************************************************
 * Software UART example for MSP430.
 *
 * Stefan Wendler
 * sw@kaltpost.de
 * http://gpio.kaltpost.de
 *
 * Echos back each character received enclosed in square brackets "[" and "]".
 * Use /dev/ttyACM0 at 9600 Bauds (and 8,N,1).
 ******************************************************************************/

#include <msp430.h>
#include <stdint.h>

#include "uart.h"

/**
 * Main routine
 */
uint8_t q = 25;
int main(void)
{
     WDTCTL = WDTPW + WDTHOLD; 	// Stop WDT

     BCSCTL1 = CALBC1_1MHZ; 		// Set range
     DCOCTL = CALDCO_1MHZ; 		// SMCLK = DCO = 1MHz

     uart_init();

     __enable_interrupt();

     uart_puts("\n\r***************\n\r");

     uart_puts("***************\n\r\n\r");



     while(1) {
          if(uart_getc(&q)) {

          }
          uart_puts("MSP430 softuart\n\r");
     }
}

