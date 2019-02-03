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

void BMP280::setup(void) {
    uint8_t sensor_id = get_id();
    if(sensor_id == 0x00)
        os_printf("COULD NOT CONNECT TO BMP280 - ID IS 0x00.\r\n");
    else {
        os_printf("FOUND BMP280 WITH ID: ");
        os_printf("%d.\r\n", sensor_id);

        uint8_t ctrl_meas_reg_data = SENSOR_TEMP_SAMPLING_16 | SENSOR_PRESSURE_SKIP | SENSOR_NORMAL_MODE;
        uint8_t read_back[1];
        write_reg(SENSOR_CTRL_MEAS_REG, ctrl_meas_reg_data);
        os_delay_us(100);
        read_reg(SENSOR_CTRL_MEAS_REG, 1, read_back);
        if(read_back[0] == ctrl_meas_reg_data)
            os_printf("SUCCESSFULLY CONFIGURED: TEMP SAMPLING 16, PRESSURE SKIP, NORMAL MODE.\r\n");
        else
            os_printf("FAILED TO CONFIGURE CTRL_MEAS_REG.\r\n");
    }
}

uint8_t BMP280::get_id(void) {
    uint8_t sensor_id[1];
    read_reg(SENSOR_ID_REG, 1, sensor_id);
    return sensor_id[0];
}

void BMP280::write_reg(uint8_t reg_addr, uint8_t data) {
    i2c.start();

    if(i2c.send_byte(SLAVE_WRITE)) {
        if(i2c.send_byte(reg_addr)) {
            i2c.send_byte(data);
        }
    }

    i2c.stop();
}

void BMP280::read_reg(uint8_t reg_addr, uint8_t amount, uint8_t reg_data[]) {
    i2c.start();

    if(i2c.send_byte(SLAVE_WRITE)) {
        if(i2c.send_byte(reg_addr)) {
            i2c.repeated_start();
            if(i2c.send_byte(SLAVE_READ)) {
                for(uint8_t i = 0; i < amount; i++) {
                    uint8_t data = i2c.read_byte();
                    if(i == amount - 1)
                        i2c.master_nack();
                    else
                        i2c.master_ack();
                    reg_data[i] = data;
                }
            }
        }
    }

    i2c.stop();
}