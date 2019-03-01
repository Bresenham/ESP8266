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