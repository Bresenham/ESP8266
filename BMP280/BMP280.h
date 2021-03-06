#ifndef BMP280_H
#define BMP280_H

#include "../Soft_I2C/I2C.h"

/*
 * For these two addresses to work, the SDO-Pin of the BMP280 needs to be connected to VCC (=high).
 * SDO_GPIO defines the PIN on the ESP8266, the SDO-PIN of the BMP280 needs to be connected to.
*/
#define SLAVE_WRITE                     0xEE
#define SLAVE_READ                      0xEF

#define SENSOR_ID_REG                   0xD0

#define SENSOR_STATUS_REG               0xF3
#define SENSOR_STATUS_MEASURING         0x08

#define SENSOR_CTRL_MEAS_REG            0xF4

#define SENSOR_CONFIG_REG               0xF5
#define SENSOR_STANDBY_TIME_0_5_MS      0x00
#define SENSOR_STANDBY_TIME_500_MS      0x80
#define SENSOR_STANDBY_TIME_1000_MS     0xA0
#define SENSOR_STANDBY_TIME_2000_MS     0xC0

#define SENSOR_TEMP_SAMPLING_16         0xE0
#define SENSOR_TEMP_SAMPLING_8          0x80
#define SENSOR_TEMP_SAMPLING_4          0x60
#define SENSOR_TEMP_SAMPLING_2          0x40
#define SENSOR_TEMP_SAMPLING_1          0x20

#define SENSOR_TEMP_MEASURE_MSB         0xFA
#define SENSOR_TEMP_MEASURE_LSB         0xFB
#define SENSOR_TEMP_MEASURE_XLSB        0xFC

#define SENSOR_PRESSURE_SKIP            0x00

#define SENSOR_SLEEP_MODE               0x00
#define SENSOR_FORCE_MODE               0x01
#define SENSOR_NORMAL_MODE              0x03

#define SENSOR_DIG_T1_LSB_REG           0x88
#define SENSOR_DIG_T1_MSB_REG           0x89

#define SENSOR_DIG_T2_LSB_REG           0x8A
#define SENSOR_DIG_T2_MSB_REG           0x8B

#define SENSOR_DIG_T3_LSB_REG           0x8C
#define SENSOR_DIG_T3_MSB_REG           0x8D

class BMP280 {
    public:
        void test(void);
        int32_t read_temperature(void);
        BMP280(void);
    private:
        void setup(void);
        int32_t calculate_temperature(uint32_t value);
        uint8_t get_id(void);
        void read_reg(uint8_t reg_addr, uint8_t amount, uint8_t reg_data[]);
        uint8_t read_reg(uint8_t reg_addr);
        void write_reg(uint8_t reg_addr, uint8_t data);
        uint16_t dig_T1;
        int16_t dig_T2;
        int16_t dig_T3;
        I2C i2c;
};

#endif