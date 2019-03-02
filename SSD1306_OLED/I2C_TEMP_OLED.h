#ifndef I2C_TEMP_OLED_H
#define I2C_TEMP_OLED_H

#include "I2C_OLED.h"
#include "../Buffer/Buffer.h"

class I2CTemperatureDisplay : public I2CDisplay {
    public:
        void push_temperature_value(int32_t value);
        void draw_temperature_graph(void);
        void set_upper_temp_limit(int16_t upper_limit) { this->upper_limit = upper_limit; };
        void set_lower_temp_limit(int16_t lower_limit) { this->lower_limit = lower_limit; };
        I2CTemperatureDisplay() : I2CDisplay() { buffer = Buffer(); };
    private:
        Buffer buffer;
        int16_t lower_limit;
        int16_t upper_limit;
};

#endif
