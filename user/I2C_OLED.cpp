#include "I2C.h"
#include "I2C_OLED.h"

const uint8_t ssd1306_init_sequence [] = {	// Initialization Sequence
    0xAE,			// Display OFF (sleep mode)
    0x20, 0b00,		// Set Memory Addressing Mode
                    // 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
                    // 10=Page Addressing Mode (RESET); 11=Invalid
    0xB0,			// Set Page Start Address for Page Addressing Mode, 0-7
    0xC8,			// Set COM Output Scan Direction
    0x00,			// --set low column address
    0x10,			// --set high column address
    0x40,			// --set start line address
    0x81, 0x3F,		// Set contrast control register
    0xA1,			// Set Segment Re-map. A0=address mapped; A1=address 127 mapped. 
    0xA6,			// Set display mode. A6=Normal; A7=Inverse
    0xA8, 0x3F,		// Set multiplex ratio(1 to 64)
    0xA4,			// Output RAM to Display
                    // 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
    0xD3, 0x00,		// Set display offset. 00 = no offset
    0xD5,			// --set display clock divide ratio/oscillator frequency
    0xF0,			// --set divide ratio
    0xD9, 0x22,		// Set pre-charge period
    0xDA, 0x12,		// Set com pins hardware configuration		
    0xDB,			// --set vcomh
    0x20,			// 0x20,0.77xVcc
    0x8D, 0x14,		// Set DC-DC enable
    0xAF,			// Display ON in normal mode

};

I2CDisplay::I2CDisplay(void) {
    I2CDisplay(128, 96);
}

I2CDisplay::I2CDisplay(uint8_t width, uint8_t height) {
    this->setWidth(width);
    this->setHeight(height);
    i2c = I2C();
}

void I2CDisplay::init(void) {
    i2c.start();
    for(uint8_t i = 0; i < sizeof(ssd1306_init_sequence); i++)
        lcd_command(ssd1306_init_sequence[i]);
    lcd_command(LCD_DISP_ON);
}

void I2CDisplay::clear_screen(void) {
    goto_x_y(0, 0);
    i2c.start();
    i2c.send_byte(LCD_I2C_ADDR);
    i2c.send_byte(0x40);
    for(uint16_t i = 0; i < 128 * 8; i++)
        i2c.send_byte(0x00);
    i2c.stop();
}

void I2CDisplay::goto_x_y(uint8_t x, uint8_t y) {
    i2c.start();
    i2c.send_byte(LCD_I2C_ADDR);
    i2c.send_byte(0x00);
    i2c.send_byte(0xb0 + y);
    i2c.send_byte(0x21);
    i2c.send_byte(x);
    i2c.send_byte(0x7F);
    i2c.stop();
}

void I2CDisplay::lcd_command(uint8_t cmd) {
    i2c.start();
    i2c.send_byte(LCD_I2C_ADDR);
    i2c.send_byte(0x00);
    i2c.send_byte(cmd);
    i2c.stop();
}

void I2CDisplay::setWidth(uint8_t width) {
    this->width = width;
}

void I2CDisplay::setHeight(uint8_t height) {
    this->height = height;
}