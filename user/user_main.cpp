extern "C" {
    #include "user_interface.h"
    #include "ets_sys.h"
    #include "osapi.h"
}

#include "../BMP280/BMP280.h"
#include "../SSD1306_OLED/I2C_TEMP_OLED.h"
#include "../WIFI/Wifi.h"
#include "../Util/Util.h"

#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0xfb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0xfc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0xfd000
#elif (SPI_FLASH_SIZE_MAP == 3)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 4)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#elif (SPI_FLASH_SIZE_MAP == 5)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 6)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#else
#error "The flash map is not supported"
#endif

static const partition_item_t at_partition_table[] = {
    { SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
    { SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
    { SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
};

extern "C" void ICACHE_FLASH_ATTR user_pre_init(void) {
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while(1);
	}
}

os_timer_t temperature_timer;
os_timer_t init_timer;

BMP280 bmp_280;
I2CTemperatureDisplay oled;
Wifi wifi;

bool is_connected = false;

extern "C" void ICACHE_FLASH_ATTR wifi_not_connected(void) {
    is_connected = false;
}

extern "C" void ICACHE_FLASH_ATTR wifi_connected(void) {
    is_connected = true;
}

extern "C" void ICACHE_FLASH_ATTR scan_done_cb(void *arg, STATUS status) {
    if(status == OK) {
        char buf[100];
        struct bss_info *bssInfo = (struct bss_info*)arg;
        bssInfo = STAILQ_NEXT(bssInfo, next);

        os_printf("-------------- WIFI SCAN RESULTS --------------\r\n");
        while (bssInfo != NULL) {
            os_sprintf(buf, "%-32s %02X:%02X:%02X:%02X:%02X:%02X, ch %2d, auth %d, hid %d, rssi %d\n\r", 
                    bssInfo->ssid, 
                    bssInfo->bssid[0], bssInfo->bssid[1], bssInfo->bssid[2],
                    bssInfo->bssid[3], bssInfo->bssid[4], bssInfo->bssid[5],
                    bssInfo->channel,
                    bssInfo->authmode, bssInfo->is_hidden,
                    bssInfo->rssi);
            os_printf(buf);
            os_delay_us(200);
            bssInfo = STAILQ_NEXT(bssInfo, next);
        }
        os_printf("-------------- END --------------\r\n");
    } else
        os_printf("-------------- SCAN RESULT NOT OK --------------\r\n");
}

extern "C" void ICACHE_FLASH_ATTR read_temperature(void *ptr) {
    const int32_t temperature = bmp_280.read_temperature();

    if(temperature < 4000 && temperature > -2000) {

        oled.push_temperature_value(temperature);
        oled.draw_temperature_graph();

        if(is_connected)
            oled.fill_rect(119, 1, 124, 5);    
        else
            oled.draw_rect(119, 1, 124, 6);
        oled.display();

        char data_to_send[11];
        os_sprintf(data_to_send, "VALUE=%d", temperature);
        os_printf(data_to_send);
        os_printf("\r\n");

        wifi.send_data(data_to_send);
    } else
        os_printf("TEMPERATURE VALUE OUT OF RANGE\r\n");
}

extern "C" void ICACHE_FLASH_ATTR init_classes(void *ptr) {
    wifi = Wifi();
    wifi.register_connected_cb(wifi_connected);
    wifi.register_could_not_connect_cb(wifi_not_connected);

    bmp_280 = BMP280();
    
    oled = I2CTemperatureDisplay();

    os_timer_setfn(&temperature_timer, read_temperature, NULL);
    os_timer_arm(&temperature_timer, 1000, true);
}

extern "C" void ICACHE_FLASH_ATTR user_init(void) {
    os_timer_setfn(&init_timer, init_classes, NULL);
    os_timer_arm(&init_timer, 0, false);
}
