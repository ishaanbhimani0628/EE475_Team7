#include <msp430.h>
#include <stdint.h>
#include "adxl362.h"

/**
 * adxl362.c
 */


void ADXL362_configSetupBegin() {
    // config
    P1DIR    |= BIT4;                                        // Configure P1.4 (BIT4) as output
    P1OUT    |= BIT4;                                        // Set CS

    USICTL0   = USISWRST;                                    // USI in reset state
    USICTL0  |= (USIPE7 | USIPE6 | USIPE5 | USIMST | USIOE); // SDI, SDO and CLK enabled, MSB first, master mode, output enabled
    USICKCTL  = (USIDIV_4 | USISSEL_2);                      // Clock divider 16, SMCLK, clock idles low (changed on 1st, captured on 2nd edge)
    USICKCTL &= ~(USICKPL); // set CPOL to 0 to match ADXL362
    USICTL1 |= USICKPH; //change CPHA to 0 to match ADXL362
    USICTL0  &= ~USISWRST;                                   // USI released from reset

    // setup
    //  1. Write 250 decimal (0xFA) to Register 0x20, and write 0 to
    //  Register 0x21: sets activity threshold to 250 mg.
//    ADXL362_SPIWriteOneReg(0x20, 0xFA);
//    ADXL362_SPIWriteOneReg(0x21, 0x00);

    ADXL362_SPIWriteOneReg(0x1F, 0x52);

    // begin
    uint8_t temp = ADXL362_SPIReadOneReg(0x2D) | 0x02; // read power_ctl reg
    ADXL362_SPIWriteOneReg(0x2D, temp);
    delay_ms(10);
}


void ADXL362_ConfigSPI()
{
  P1DIR    |= BIT4;                                        // Configure P1.4 (BIT4) as output
  P1OUT    |= BIT4;                                        // Set CS

  USICTL0   = USISWRST;                                    // USI in reset state
  USICTL0  |= (USIPE7 | USIPE6 | USIPE5 | USIMST | USIOE); // SDI, SDO and CLK enabled, MSB first, master mode, output enabled
  USICKCTL  = (USIDIV_4 | USISSEL_2);                      // Clock divider 16, SMCLK, clock idles low (changed on 1st, captured on 2nd edge)
  USICKCTL &= ~(USICKPL); // set CPOL to 0 to match ADXL362
  USICTL1 |= USICKPH; //change CPHA to 0 to match ADXL362
  USICTL0  &= ~USISWRST;                                   // USI released from reset
}

void ADXL362_setup() {
    //  1. Write 250 decimal (0xFA) to Register 0x20, and write 0 to
    //  Register 0x21: sets activity threshold to 250 mg.
    ADXL362_SPIWriteOneReg(0x20, 0xFA);
    ADXL362_SPIWriteOneReg(0x21, 0x00);

    ADXL362_SPIWriteOneReg(0x1F, 0x52);
}

void ADXL362_beginMeasure() {
    uint8_t temp = ADXL362_SPIReadOneReg(0x2D) | 0x02; // read power_ctl reg
    ADXL362_SPIWriteOneReg(0x2D, temp);
    delay_ms(10);
}

uint8_t ADXL362_readXYZData() {
    // read x data
    return ADXL362_SPIReadOneReg(0x0E);

//    // read y= data
//    *ydata = ADXL362_SPIReadOneReg(0x10);
//
//    // read z data
//    *zdata = ADXL362_SPIReadOneReg(0x12);
}

uint8_t ADXL362_SPIReadOneReg(uint8_t regAddress) {
    P1OUT &= ~(BIT4); // set CS low
    USISR = 0x0B;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = regAddress;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x0;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    uint8_t recvd = USISR;
    P1OUT |= BIT4;
    return recvd;
}

void ADXL362_SPIWriteOneReg(uint8_t regAddress, uint8_t regValue) {
    P1OUT &= ~(BIT4); // set CS low
    USISR = 0x0A;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = regAddress;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = regValue;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    P1OUT |= BIT4;
}

void delay_ms(unsigned int ms) {
    while (ms) {
        __delay_cycles(1000);// 1000 for 1MHz and 16000 for 16MHz
        ms--;
    }
}
