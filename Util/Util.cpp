extern "C" {
    #include "osapi.h"
}
#include "Util.h"

void ICACHE_FLASH_ATTR Util::print_float(const float val, char *buff) {
	char smallBuff[16];
	const int val1 = (int) val;
	unsigned int val2;
	if (val < 0) {
		val2 = (int) (-100.0 * val) % 100;
	} else {
		val2 = (int) (100.0 * val) % 100;
	}

	os_sprintf(smallBuff, "%i.%02u", val1, val2);
	os_strcpy(buff, smallBuff);
}

int32_t ICACHE_FLASH_ATTR Util::get_largest_element(const int32_t *values, const int16_t size) {
	return Util::get_compare_value(values, size, Util::compare_highest_element);
}

int32_t ICACHE_FLASH_ATTR Util::get_lowest_element(const int32_t *values, const int16_t size) {
	return Util::get_compare_value(values, size, Util::compare_lowest_element);
}

int32_t ICACHE_FLASH_ATTR Util::get_compare_value(const int32_t *values, const int16_t size, bool (*compare)(const int32_t, const int32_t)) {
	int32_t value = values[0];
	for(uint16_t i = 1; i < size; i++) {
		if((*compare)(values[i], value))
			value = values[i];
	}
	
	return value;
}

int32_t ICACHE_FLASH_ATTR Util::map(const int32_t x, const int32_t in_min, const int32_t in_max, const int32_t out_min, const int32_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool ICACHE_FLASH_ATTR Util::compare_lowest_element(const int32_t v1, const int32_t v2) { return v1 < v2; }

bool ICACHE_FLASH_ATTR Util::compare_highest_element(const int32_t v1, const int32_t v2) { return v1 > v2; }