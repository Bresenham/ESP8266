#ifndef I2C_TEMP_OLED_H
#define I2C_TEMP_OLED_H

#include "I2C_OLED.h"


class I2CTemperatureDisplay : public I2CDisplay {
    public:
        void push_temperature_value(int32_t value);
        void draw_temperature_graph(void);
        I2CTemperatureDisplay() : I2CDisplay() {};
    private:
        int32_t temp_values[20];
};

#endif
