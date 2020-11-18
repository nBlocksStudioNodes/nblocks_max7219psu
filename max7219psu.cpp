#include "MAX7219PSU.h"

nBlock_MAX7219PSU::nBlock_MAX7219PSU(PinName MOSI, PinName MISO, PinName SCK, PinName CS, uint16_t Brightness, uint16_t ScanLimit):
    _spi(MOSI, MISO, SCK, CS) {
    _spi.format(8,0);                // 8-bit format, mode 0,0
    _spi.frequency(1000000);         // SCLK = 1 MHz
	// Reset flag
	must_update = 0;
    init_MAX7219();		
}

void nBlock_MAX7219PSU::triggerInput(nBlocks_Message message){
	// input 0 receives an array of bytes
	if (message.inputNumber == 0) {
		// Copy values to internal array
		uint16_t * source_values;
		source_values = ((uint16_t *)message.pointerValue);
		Position = source_values[0];
        Value = source_values[1];		
		// Set flag so we update the MAX7219 at the end of frame
		must_update =  1;
	}
}

void nBlock_MAX7219PSU::endFrame(void){
	if (must_update) {
		must_update = 0;
		max7219_write(Value, Position, Brightness, ScanLimit);
	}	
}

void nBlock_MAX7219PSU::max7219_write(uint16_t Value, uint16_t Position, uint16_t Brightness, uint16_t ScanLimit) {
    cs = 1;                          // CS initially High 
    spi_write_2bytes(0x0A, Brightness); 
    spi_write_2bytes(0x0B, ScanLimit); 

    // Encode Value and Position to digits
    if (Position == 1) offset = 4;      // This is for Volts
    if (Position == 2) offset = 0;      // This is for Amperes
    dig[3 + offset] = (Value / 1000);
    dig[2 + offset] = (Value - dig7 * 1000) / 100;
    dig[1 + offset] = (Value - dig7 * 1000 - dig6 * 100) / 10;
    dig[0 + offset] = (Value - dig7 * 1000 - dig6 * 100 - dig5 * 10);

    // Send digit values to MAX7219
    spi_write_2bytes(0x01 + offset, dig[0 + offset]);     //digit 0 
    spi_write_2bytes(0x02 + offset, dig[1 + offset]);     //digit 1 
    spi_write_2bytes(0x03 + offset, 0x80 + dig[2 + offset]);     //digit 2 has active the decimal point
    spi_write_2bytes(0x04 + offset, dig[3 + offset]);     //digit 3 
}

void nBlock_MAX7219PSU::spi_write_2bytes(unsigned char MSB, unsigned char LSB) {
    cs = 0;                         // Set CS Low
    _spi.write(MSB);                 // Send two bytes
    _spi.write(LSB);
    cs = 1;                         // Set CS High
}

void nBlock_MAX7219PSU::init_MAX7219(void) {
    cs = 1;                               // CS initially High 
    spi_write_2bytes(0x09, 0xFF);         // Decoding off //nikos changed to full decode
    spi_write_2bytes(0x0A, 0x08);         // Brightness to intermediate
    spi_write_2bytes(0x0B, 0x07);         // Scan limit 7th digit
    spi_write_2bytes(0x0C, 0x01);         // Normal operation mode, this is the shutdown register
    spi_write_2bytes(0x0F, 0x0F);         // Enable display test

    wait_us(500000);                   // 500 ms delay

    spi_write_2bytes(0x01, 0x00);         // Clear row 0.
    spi_write_2bytes(0x02, 0x00);         // Clear row 1.
    spi_write_2bytes(0x03, 0x00);         // Clear row 2.
    spi_write_2bytes(0x04, 0x00);         // Clear row 3.
    spi_write_2bytes(0x05, 0x00);         // Clear row 4.
    spi_write_2bytes(0x06, 0x00);         // Clear row 5.
    spi_write_2bytes(0x07, 0x00);         // Clear row 6.
    spi_write_2bytes(0x08, 0x00);         // Clear row 7.
    spi_write_2bytes(0x0F, 0x00);         // Disable display test

}