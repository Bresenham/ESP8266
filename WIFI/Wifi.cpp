extern "C" {
    #include "ip_addr.h"
    #include "os_type.h"
    #include "ets_sys.h"
    #include "osapi.h"
    #include "espconn.h"
    #include "eagle_soc.h"
    #include "user_interface.h"
    #include "gpio.h"
}

#include "Wifi.h"

void Wifi::connect(void) {
    struct station_config stationConf;
    
    stationConf.bssid_set = 0;
    os_memcpy(&stationConf.ssid, this->SSID, 32);
    os_memcpy(&stationConf.password, this->PW, 64);

    wifi_station_set_config(&stationConf);
    wifi_station_connect();
}

Wifi::Wifi() {
    wifi_set_opmode(STATION_MODE);
    this->SSID = "";
    this->PW = "";
}

