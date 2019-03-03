#ifndef BUFFER_H
#define BUFFER_H

#include "stdint.h"

#define BUFFER_SIZE 128

class Buffer {
    public:
        void get_values(int32_t *cpy);
        void push_value(const int32_t value);
        uint8_t get_size(void);
        Buffer(void);
    private:
        void shift_buffer(void);
        int32_t buffer[BUFFER_SIZE];
        uint8_t size = 0;
};

#endif /* BUFFER_H */