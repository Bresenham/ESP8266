#ifndef UTIL_H
#define UTIL_H

#include "stdint.h"

class Util {
    public:
        static void printFloat(float val, char *buff);
        static int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max);
};

#endif /* UTIL_H */