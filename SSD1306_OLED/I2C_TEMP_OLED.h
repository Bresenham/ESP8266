#ifndef I2C_TEMP_OLED_H
#define I2C_TEMP_OLED_H

#include "I2C_OLED.h"
#include "../Buffer/Buffer.h"

#define SCALE_DISTANCE 20

class I2CTemperatureDisplay : public I2CDisplay {
    public:
        void push_temperature_value(const int32_t value);
        void draw_temperature_graph(void);
        I2CTemperatureDisplay() : I2CDisplay() { buffer = Buffer(); };
    private:
        Buffer buffer;
        int16_t upper_limit = 2700;
        int16_t lower_limit = 2100;
};

#endif
