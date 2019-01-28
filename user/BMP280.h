#ifndef BMP280_H
#define BMP280_H

#include "I2C.h"

class BMP280 {
    public:
        void read_temperature(void);
        BMP280();
    private:
        I2C i2c;
};

#endif