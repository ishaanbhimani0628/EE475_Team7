/**
 * adxl362.h
 */

#ifndef ADXL362_H_
#define ADXL362_H_

void ADXL362_configSetupBegin();
void ADXL362_ConfigSPI();
void ADXL362_setup();
void ADXL362_beginMeasure();
uint8_t ADXL362_readXYZData();
uint8_t ADXL362_SPIReadOneReg(uint8_t regAddress);
void ADXL362_SPIWriteOneReg(uint8_t regAddress, uint8_t regValue);
void delay_ms(unsigned int ms);


#endif /* ADXL362_H_ */
