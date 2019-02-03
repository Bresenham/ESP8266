#ifndef BMP280_H
#define BMP280_H

#include "I2C.h"

/*
 * For these addresses to work, the SDO-Pin of the BMP280 needs to be connected to VCC (=high).
 * SDO_GPIO defines the PIN on the ESP8266, the SDO-PIN of the BMP280 needs to be connected to.
*/

#define SLAVE_WRITE                     0xEE
#define SLAVE_READ                      0xEF

#define SENSOR_ID_REG                   0xD0

#define SENSOR_CTRL_MEAS_REG            0xF4

#define SENSOR_TEMP_SAMPLING_16         0xE0
#define SENSOR_TEMP_SAMPLING_8          0x40
#define SENSOR_TEMP_SAMPLING_4          0x30

#define SENSOR_PRESSURE_SKIP            0x00

#define SENSOR_SLEEP_MODE               0x00
#define SENSOR_FORCE_MODE               0x01
#define SENSOR_NORMAL_MODE              0x03

#define SDO_GPIO 2

class BMP280 {
    public:
        void read_temperature(void);
        void setup(void);
        BMP280(void);
    private:
        int32_t calculate_temperature(uint32_t adc_T);
        uint8_t get_id(void);
        void read_reg(uint8_t reg_addr, uint8_t amount, uint8_t reg_data[]);
        void write_reg(uint8_t reg_addr, uint8_t data);
        I2C i2c;

};

#endif