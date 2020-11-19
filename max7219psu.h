#ifndef __NB_MAX7219PSU
#define __NB_MAX7219PSU

#include "nworkbench.h"

class nBlock_MAX7219PSU: public nBlockSimpleNode<1> {
public:
    nBlock_MAX7219PSU(PinName MOSI, PinName MISO, PinName SCK, PinName pinCS, uint16_t Brightness, uint16_t ScanLimit);
    void triggerInput(nBlocks_Message message);
	void endFrame();
	void max7219_write1(uint16_t Value1);
    void max7219_write2(uint16_t Value2);
    void spi_write_2bytes(unsigned char MSB, unsigned char LSB);
    void init_MAX7219(uint16_t Brightness, uint16_t ScanLimit);

   
private:	
    SPI _spi;
    DigitalOut _cs;
	uint32_t Value1;
    uint32_t Value2;
    uint32_t Position1;
    uint32_t Position2;
	uint32_t must_update = 0;
    uint16_t Brightness; 
    uint16_t ScanLimit;
    uint16_t dig[8];
};

#endif