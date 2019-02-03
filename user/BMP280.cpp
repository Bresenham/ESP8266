extern "C" {
    #include "ets_sys.h"
    #include "osapi.h"
    #include "gpio.h"
}
#include "BMP280.h"

BMP280::BMP280(void) {
    i2c = I2C();
    gpio_output_set(0, SDO_GPIO, SDO_GPIO, 0);
    setup();
}

void BMP280::calculate_temperature(void) {
    uint8_t raw_sensor_data[3];

    while(read_reg(SENSOR_STATUS_REG) & SENSOR_STATUS_MEASURING);

    read_reg(SENSOR_TEMP_MEASURE_MSB, 3, raw_sensor_data);

    /* TODO: Take the 3 raw-data bytes into formula and calculate temperature */
}

void BMP280::setup(void) {
    uint8_t sensor_id = get_id();

    if(sensor_id == 0x00)
        os_printf("COULD NOT CONNECT TO BMP280 - ID IS 0x00.\r\n");
    else {
        os_printf("FOUND BMP280 WITH ID: ");
        os_printf("%d.\r\n", sensor_id);

        uint8_t ctrl_meas_reg_data = SENSOR_TEMP_SAMPLING_16 | SENSOR_PRESSURE_SKIP | SENSOR_NORMAL_MODE;
        write_reg(SENSOR_CTRL_MEAS_REG, ctrl_meas_reg_data);
        os_delay_us(100);

        if(read_reg(SENSOR_CTRL_MEAS_REG) == ctrl_meas_reg_data) {
            os_printf("SUCCESSFULLY CONFIGURED: TEMP SAMPLING 16, PRESSURE SKIP, NORMAL MODE.\r\n");

            write_reg(SENSOR_CONFIG_REG, SENSOR_STANDBY_TIME_500_MS);
            os_delay_us(100);

            if(read_reg(SENSOR_CONFIG_REG) == SENSOR_STANDBY_TIME_500_MS) {
                os_printf("SUCCESSFULLY CONFIGURED: STANDBY TIME 500MS.\r\n");

                dig_T1 = read_reg(SENSOR_DIG_T1_MSB_REG) << 8 | read_reg(SENSOR_DIG_T1_LSB_REG);
                dig_T2 = read_reg(SENSOR_DIG_T2_MSB_REG) << 8 | read_reg(SENSOR_DIG_T2_LSB_REG);
                dig_T3 = read_reg(SENSOR_DIG_T3_MSB_REG) << 8 | read_reg(SENSOR_DIG_T3_LSB_REG);

                if(dig_T1 != 0 && dig_T2 != 0 && dig_T3 != 0)
                    os_printf("SUCCESSFULLY CONFIGURED BMP280.\r\n");
                else
                    os_printf("SOME COMPENSATION VALUES ARE 0.\r\n");
            }
            else
                os_printf("FAILED TO CONFIGURE CONFIG_REG.\r\n");
        }
        else
            os_printf("FAILED TO CONFIGURE CTRL_MEAS_REG.\r\n");
    }
}

uint8_t BMP280::get_id(void) {
    return read_reg(SENSOR_ID_REG);
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

uint8_t BMP280::read_reg(uint8_t reg_addr) {
    uint8_t reg_data[1];
    read_reg(reg_addr, 1, reg_data);
    return reg_data[0];
}