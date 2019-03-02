extern "C" {
    #include "ets_sys.h"
    #include "osapi.h"
    #include "gpio.h"
}

#include "Buffer.h"

Buffer::Buffer(void) {
    os_memset(buffer, 0, BUFFER_SIZE);
    size = 0;
}

void Buffer::push_value(int32_t value) {
    shift_buffer();
    buffer[0] = value;
    if(size < BUFFER_SIZE - 1)
        size++;
}

void Buffer::shift_buffer(void) {
    if(size == 0) return;
    
    for(uint8_t i = size; i > 0; i--) {
        buffer[i] = buffer[i - 1];
    }
}

void Buffer::get_values(int32_t *copy) {
    for(uint8_t i = 0; i < size; i++) {
        copy[i] = buffer[i];
    }
}

uint8_t Buffer::get_size(void) {
    return size;
}