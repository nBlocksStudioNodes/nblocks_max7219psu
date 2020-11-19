#ifndef __NB_MAX7219PSU
#define __NB_MAX7219PSU

#include "nworkbench.h"

class nBlock_MAX7219PSU: public nBlockSimpleNode<1> {
public:
    nBlock_MAX7219PSU(PinName MOSI, PinName MISO, PinName SCK, PinName CS, uint16_t Brightness, uint16_t ScanLimit);
    void triggerInput(nBlocks_Message message);
	void endFrame();
	void max7219_write(uint16_t Value, uint16_t Position, uint16_t Brightness, uint16_t ScanLimit);
    void spi_write_2bytes(unsigned char MSB, unsigned char LSB);
    void init_MAX7219(void);

   
private:	
    SPI _spi;
    DigitalInOut _cs;
	uint32_t Value;
    uint32_t Position;
	uint32_t must_update = 0;
    uint16_t Brightness; 
    uint16_t ScanLimit;
    uint16_t dig[8];
    uint16_t offset;
};

#endif