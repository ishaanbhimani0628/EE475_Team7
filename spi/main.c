#include <msp430.h> 
#include <stdint.h>

/**
 * main.c
 */

void ConfigSPI(void);
uint16_t SpiWriteByte( uint16_t write_data);
void delay_ms(unsigned int ms);
uint16_t SpiReadAccel(uint16_t cmd);
void ADXL362_begin();
uint16_t readXData();
void ADXL362_beginMeasure();
void readXYZData(int16_t* xdata, int16_t* ydata, int16_t* zdata, int16_t* temp);



volatile uint16_t read_data = 0;
volatile int16_t x_data = 0;
volatile int16_t y_data = 0;
volatile int16_t z_data = 0;
volatile int16_t t_data = 0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	ConfigSPI();
	ADXL362_begin();
	ADXL362_beginMeasure();
	while(1) {
	    readXYZData(&x_data, &y_data, &z_data, &t_data);
	}
}

void ConfigSPI( void )
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

void ADXL362_begin() {
    P1OUT &= ~(BIT4); // set CS low
    USISR = 0x0A;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x1F;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x52;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    P1OUT |= BIT4;
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

void ADXL362_beginMeasure() {
    uint8_t temp = ADXL362_SPIReadOneReg(0x2D); // read power_ctl reg
    uint8_t tempwrite = temp | 0x02;
    ADXL362_SPIWriteOneReg(0x2D, tempwrite);
    delay_ms(10);
}

uint16_t readXData() {
    P1OUT &= ~(BIT4); // set CS low
    USISR = 0x0B;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x0E;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x0;
    USICNT = (USI16B | 16);
    while( !(USICTL1 & USIIFG) );
    uint16_t xdata = USISR;
    P1OUT |= BIT4;
    return xdata;
}

void readXYZData(int16_t* xdata, int16_t* ydata, int16_t* zdata, int16_t* temp) {
    P1OUT &= ~(BIT4); // set CS low

    // read x data
    USISR = 0x0B;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x0E;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x0;
    USICNT = (USI16B | 16);
    while( !(USICTL1 & USIIFG) );
    *xdata = USISR;

    // read y= data
    USISR = 0x0B;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x10;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x0;
    USICNT = (USI16B | 16);
    while( !(USICTL1 & USIIFG) );
    *ydata = USISR;

    // read z data
    USISR = 0x0B;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x12;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x0;
    USICNT = (USI16B | 16);
    while( !(USICTL1 & USIIFG) );
    *zdata = USISR;

    // read y data
    USISR = 0x0B;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x14;
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    USISR = 0x0;
    USICNT = (USI16B | 16);
    while( !(USICTL1 & USIIFG) );
    *temp = USISR;

    P1OUT |= BIT4;
}



uint16_t SpiReadAccel(uint16_t cmd) {
    P1OUT &= ~(BIT4); // set CS low
    USISR = cmd;          // writ16 8 bits
    USICNT = (USI16B | 16);  // Start transfer of 16 bits - clears IFG
    while( !(USICTL1 & USIIFG) ); // Wait until transfer complete
    USICNT = 8;
    while( !(USICTL1 & USIIFG) );
    uint16_t read_data = USISR;
    P1OUT |= BIT4;                // Set CS high
    return read_data;
}

uint16_t SpiWriteByte(uint16_t write_data)
{
  P1OUT &= ~(BIT4); // set CS low
  USISR = write_data;          // write 8 bits
  USICNT = 8;                  // Start transfer of 8 bits - clears IFG
  while( !(USICTL1 & USIIFG) ); // Wait until transfer complete
  uint16_t read_data = USISR;
  P1OUT |= BIT4;                // Set CS high
  return read_data;
}

void delay_ms(unsigned int ms)
{
    while (ms)
    {
        __delay_cycles(1000);// 1000 for 1MHz and 16000 for 16MHz
        ms--;
    }
}
