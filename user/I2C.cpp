extern "C" {
    #include "ets_sys.h"
    #include "osapi.h"
    #include "gpio.h"
}

#include "I2C.h"

I2C::I2C(void) {
    GPIO_REG_WRITE(
        GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_MASTER_SDA_GPIO)),
        GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_MASTER_SDA_GPIO)))
        | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)); //open drain;

    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS,
        GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_MASTER_SDA_GPIO));

    GPIO_REG_WRITE(
        GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_MASTER_SCL_GPIO)),
        GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(I2C_MASTER_SCL_GPIO)))
        | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)); //open drain;

    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS,
        GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << I2C_MASTER_SCL_GPIO));

    set_SDA_high();
    set_SCL_high();
    os_delay_us(DELAY);
}

void I2C::start(void) {
    set_SDA_low();
    os_delay_us(DELAY);
    set_SCL_low();
}

void I2C::repeated_start(void) {
    set_SDA_high();
    os_delay_us(DELAY);
    set_SCL_high();
    os_delay_us(DELAY);
    set_SDA_low();
    os_delay_us(DELAY);
    set_SCL_low();
}

void I2C::master_ack(void) {
    set_SDA_low();
    os_delay_us(DELAY);
    set_SCL_low();
    os_delay_us(DELAY);
    set_SCL_high();
    os_delay_us(DELAY);
    set_SDA_high();
}

void I2C::master_nack(void) {
    set_SDA_high();
    os_delay_us(DELAY);
    set_SCL_low();
    os_delay_us(DELAY);
    set_SCL_high();
    os_delay_us(DELAY);
    set_SDA_high();
}

void I2C::stop(void) {
    set_SCL_high();
    os_delay_us(DELAY);
    set_SDA_high();
}

uint8_t I2C::read_byte(void) {
    uint8_t result = 0x00;
    for(uint8_t i = 0; i < 9; i++) {
        set_SCL_low();
        os_delay_us(DELAY);
        set_SCL_high();
        if(read_SDA())
            result |= (1 << i);
        os_delay_us(DELAY);
    }
    return result;
}

bool I2C::send_byte(uint8_t byte) {
    for(uint8_t i = 0; i < 8; i++) {
        set_SCL_low();
        if(byte & (1 << 7 - i))
            set_SDA_high();
        else
            set_SDA_low();
        os_delay_us(DELAY);
        set_SCL_high();
        os_delay_us(DELAY);
    }

    /* 9th clock for slave to ack transmitted byte */
    set_SCL_low();
    os_delay_us(DELAY);
    set_SCL_high();
    os_delay_us(DELAY);
    set_SCL_low();
    os_delay_us(DELAY);

    return read_SDA();
}

bool I2C::read_SDA(void) {
    return GPIO_INPUT_GET(I2C_MASTER_SDA_GPIO) == 0;
}

void I2C::set_SDA_high(void) {
    gpio_output_set(1 << I2C_MASTER_SDA_GPIO, 0, 1 << I2C_MASTER_SDA_GPIO, 0);
}

void I2C::set_SDA_low(void) {
    gpio_output_set(0, 1 << I2C_MASTER_SDA_GPIO, 1 << I2C_MASTER_SDA_GPIO, 0);
}

void I2C::set_SCL_high(void) {
    gpio_output_set(1 << I2C_MASTER_SCL_GPIO, 0, 1 << I2C_MASTER_SCL_GPIO, 0);
}

void I2C::set_SCL_low(void) {
    gpio_output_set(0, 1 << I2C_MASTER_SCL_GPIO, 1 << I2C_MASTER_SCL_GPIO, 0);
}