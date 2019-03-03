#ifndef UTIL_H
#define UTIL_H

#include "stdint.h"

class Util {
    public:
        static void print_float(const float val, char *buff);
        static int32_t get_largest_element(const int32_t *values, const int16_t size);
        static int32_t get_lowest_element(const int32_t *values, const int16_t size);
        static int32_t map(const int32_t x, const int32_t in_min, const int32_t in_max, const int32_t out_min, const int32_t out_max);
    private:
        static int32_t get_compare_value(const int32_t *values, const int16_t size, bool (*compare)(const int32_t, const int32_t));
        static bool compare_lowest_element(const int32_t v1, const int32_t v2);
        static bool compare_highest_element(const int32_t v1, const int32_t v2);
};

#endif /* UTIL_H */