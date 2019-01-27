#ifndef I2C_OLED_H
#define I2C_OLED_H

extern "C" {
    #include "c_types.h"
    #include "osapi.h"
    #include "I2C.h"
}

#define LCD_DISP_ON     0xAF
#define LCD_DISP_OFF    0xAE
#define LCD_I2C_ADDR    0x78
#define LCD_INIT_I2C    true

class I2CDisplay {
    public:
        void setWidth(uint8_t width);
        void setHeight(uint8_t height);
        void init(void);
        void clear_screen(void);
        void goto_x_y(uint8_t x, uint8_t y);
        I2CDisplay();
        I2CDisplay(uint8_t width, uint8_t height);
    private:
        uint8_t width;
        uint8_t height;
        I2C i2c;
        void lcd_command(uint8_t cmd);
        void lcd_data(uint8_t data);
};

#endif