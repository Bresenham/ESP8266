#ifndef BMP280_H
#define BMP280_H

#include "I2C.h"

/*
 * For these addresses to work, the SDO-Pin of the BMP280 needs to be connected to VCC (=high).
 * SDO_GPIO defines the PIN on the ESP8266, the SDO-PIN of the BMP280 needs to be connected to.
*/

#define SLAVE_WRITE     0xEE
#define SLAVE_READ      0xEF
#define SENSOR_ID_REG   0xD0

#define SDO_GPIO 2

class BMP280 {
    public:
        void read_temperature(void);
        uint8_t setup(void);
        BMP280(void);
    private:
        int32_t calculate_temperature(uint32_t adc_T);
        uint32_t read(uint8_t reg_addr, uint8_t amount);
        I2C i2c;

};

#endif