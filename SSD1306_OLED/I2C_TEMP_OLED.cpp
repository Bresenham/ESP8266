#include "I2C_TEMP_OLED.h"
#include "../Util/Util.h"

void ICACHE_FLASH_ATTR I2CTemperatureDisplay::push_temperature_value(const int32_t value) {
    buffer.push_value(value);
}

void ICACHE_FLASH_ATTR I2CTemperatureDisplay::draw_temperature_graph(void) {
    int32_t values[buffer.get_size()];
    buffer.get_values(values);

    lower_limit = Util::get_lowest_element(values, buffer.get_size()) - SCALE_DISTANCE;
    upper_limit = Util::get_largest_element(values, buffer.get_size()) + SCALE_DISTANCE;

    char str[12];
    char float_to_str[6];
    Util::print_float(values[0] / 100.0f, float_to_str);
    os_sprintf(str, "%s�C", float_to_str);

    I2CDisplay::clear_screen();

    I2CDisplay::goto_x_y(85, 7);

    I2CDisplay::put_s(str);

    I2CDisplay::draw_line(0, 0, 0, 63);

    I2CDisplay::draw_line(0, 63, 128, 63);

    for(uint8_t i = 0; i < buffer.get_size() - 1; i++) {
        const int16_t mapped = Util::map(values[i], lower_limit, upper_limit, 0, DISPLAY_HEIGHT);
        const int16_t next_mapped = Util::map(values[i + 1], lower_limit, upper_limit, 0, DISPLAY_HEIGHT);
        I2CDisplay::draw_line(i + 1, DISPLAY_HEIGHT - mapped, i + 2, DISPLAY_HEIGHT - next_mapped);
    }

    I2CDisplay::display();
}