extern "C" {
    #include "ets_sys.h"
    #include "osapi.h"
    #include "gpio.h"
}

#include "Buffer.h"

Buffer::Buffer(void) {
    os_memset(buffer, 0, BUFFER_SIZE);
}

void ICACHE_FLASH_ATTR Buffer::push_value(const int32_t value) {
    shift_buffer();
    buffer[0] = value;
    if(size < BUFFER_SIZE - 1)
        size++;
}

void ICACHE_FLASH_ATTR Buffer::shift_buffer(void) {
    if(size == 0) return;
    
    for(uint8_t i = size; i > 0; i--) {
        buffer[i] = buffer[i - 1];
    }

    os_printf("--------- BUFFER PRINT ---------\r\n");
    for(uint8_t i = 0; i < size; i++)
        os_printf("%d, ", buffer[i]);
    os_printf("\r\n--------- BUFFER END ---------\r\n");
}

void ICACHE_FLASH_ATTR Buffer::get_values(int32_t *copy) {
    for(uint8_t i = 0; i < size; i++) {
        copy[i] = buffer[i];
    }
}

uint8_t ICACHE_FLASH_ATTR Buffer::get_size(void) {
    return size;
}