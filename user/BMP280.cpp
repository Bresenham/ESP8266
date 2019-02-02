extern "C" {
    #include "ets_sys.h"
    #include "osapi.h"
    #include "gpio.h"
}
#include "BMP280.h"

BMP280::BMP280(void) {
    i2c = I2C();
    gpio_output_set(0, SDO_GPIO, SDO_GPIO, 0);
}

uint8_t BMP280::setup(void) {
    uint8_t sensor_id = 0x00;
    i2c.start();
    if(i2c.send_byte(SLAVE_WRITE)) {
        if(i2c.send_byte(SENSOR_ID_REG)) {
            i2c.repeated_start();
            if(i2c.send_byte(SLAVE_READ)) {
                sensor_id = i2c.read_byte();
                i2c.master_nack();
            }
        }
    }
    i2c.stop();

    return sensor_id;
}