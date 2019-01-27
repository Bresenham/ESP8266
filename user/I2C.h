#ifndef I2C_H
#define I2C_H

#include "c_types.h"

#define I2C_MASTER_SDA_GPIO 4
#define I2C_MASTER_SCL_GPIO 5
#define I2C_MASTER_SDA_FUNC FUNC_GPIO4
#define I2C_MASTER_SCL_FUNC FUNC_GPIO5

#define DELAY 100

class I2C {
    public:
        void stop(void);
        bool send_byte(uint8_t byte);
        void start(void);
        I2C();
    private:
        void set_SDA_high(void);
        void set_SDA_low(void);
        void set_SCL_high(void);
        void set_SCL_low(void);
        bool read_SDA(void);
};

#endif