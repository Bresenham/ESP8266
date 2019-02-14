#ifndef I2C_OLED_H
#define I2C_OLED_H

extern "C" {
    #include "c_types.h"
    #include "osapi.h"
}

#include "../Soft_I2C/I2C.h"

#define	PROGMEM			ICACHE_RODATA_ATTR
#define LCD_DISP_ON     0xAF
#define LCD_DISP_OFF    0xAE
#define LCD_I2C_ADDR    0x78
#define LCD_INIT_I2C    true

class I2CDisplay {
    public:
        void init(void);
        void clear_screen(void);
        void goto_x_y(uint8_t x, uint8_t y);
        void put_c(char c);
        void put_s(const char* s);
        void draw_pixel(uint8_t x, uint8_t y);
        void draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
        void display(void);
        I2CDisplay() { init(); };
    protected:
        I2C i2c;
        void lcd_command(uint8_t cmd);
        void lcd_data(uint8_t data);
};

#endif