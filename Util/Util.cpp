extern "C" {
    #include "osapi.h"
}
#include "Util.h"

void Util::printFloat(float val, char *buff) {
	char smallBuff[16];
	int val1 = (int) val;
	unsigned int val2;
	if (val < 0) {
		val2 = (int) (-100.0 * val) % 100;
	} else {
		val2 = (int) (100.0 * val) % 100;
	}
	os_sprintf(smallBuff, "%i.%02u", val1, val2);
    os_strcpy(buff, smallBuff);
}

int16_t Util::map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}