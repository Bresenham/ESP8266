extern "C" {
    #include "ip_addr.h"
    #include "os_type.h"
    #include "ets_sys.h"
    #include "osapi.h"
    #include "espconn.h"
    #include "eagle_soc.h"
    #include "user_interface.h"
    #include "gpio.h"
    #include "mem.h"
}

#include "Wifi.h"

#define REQUEST_TEXT "GET /script.php HTTP/1.1\r\ncache-control: no-cache\r\nUser-Agent: curl/7.37.0\r\nAccept: */*\r\nHost: scalar.heliohost.org\r\naccept-encoding: gzip, deflate\r\n\r\n"
#define PACKET_SIZE (2 * 1024)

LOCAL os_timer_t test_timer;
LOCAL struct espconn user_tcp_conn;
LOCAL struct _esp_tcp user_tcp;
ip_addr_t tcp_server_ip;

Wifi::Wifi() {
    this->SSID = "";
    this->PW = "";
    wifi_set_opmode(STATION_MODE);
}

void ICACHE_FLASH_ATTR Wifi::connect(void) {
    // Wifi configuration 
    struct station_config stationConf; 
        
    os_memset(stationConf.ssid, 0, 32);
    os_memset(stationConf.password, 0, 64);
        
    // No MAC-specific scanning
    stationConf.bssid_set = 0; 
        
    //Set ap settings 
    os_memcpy(&stationConf.ssid, this->SSID, 32); 
    os_memcpy(&stationConf.password, this->PW, 64); 
    wifi_station_set_config(&stationConf); 

    // Set timer to check whether router allotted an IP 
    os_timer_disarm(&test_timer);
    os_timer_setfn(&test_timer, (os_timer_func_t *)user_check_ip, NULL);
    os_timer_arm(&test_timer, 100, 0);
}

void ICACHE_FLASH_ATTR Wifi::scan_done_cb(void *arg, STATUS status) {
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
}

void ICACHE_FLASH_ATTR Wifi::request_scan(void) {
    struct scan_config sc;
    sc.ssid = NULL;
    sc.bssid = NULL;
    sc.channel = 0;
    sc.show_hidden = 1;

    wifi_station_scan(&sc, scan_done_cb);
}

void ICACHE_FLASH_ATTR Wifi::user_tcp_recv_cb(void *arg, char *pusrdata, unsigned short length) {
    os_printf("Received data string: %s \r\n", pusrdata);
}

void ICACHE_FLASH_ATTR Wifi::user_tcp_sent_cb(void *arg) {
    os_printf("Sent callback: data sent successfully.\r\n");
}

void ICACHE_FLASH_ATTR Wifi::user_tcp_discon_cb(void *arg) {
    os_printf("Disconnected from server.\r\n");
}

void ICACHE_FLASH_ATTR Wifi::user_send_data(espconn *pespconn) {
    char *pbuf = (char *)os_zalloc(PACKET_SIZE);
  
    os_strcpy(pbuf, REQUEST_TEXT);

    os_printf("-------------- HTTP REQUEST CONTENT --------------\r\n");
    os_printf(pbuf);
    os_printf("-------------- END --------------\r\n");
  
    espconn_send(pespconn, (uint8_t *)pbuf, os_strlen(pbuf));
     
    os_free(pbuf);
}

void ICACHE_FLASH_ATTR Wifi::user_tcp_connect_cb(void *arg) {
    struct espconn *pespconn = (struct espconn *)arg;
  
    os_printf("Connected to server...\r\n");
  
    espconn_regist_recvcb(pespconn, user_tcp_recv_cb);
    espconn_regist_sentcb(pespconn, user_tcp_sent_cb);
    espconn_regist_disconcb(pespconn, user_tcp_discon_cb);
     
    user_send_data(pespconn);
}

void ICACHE_FLASH_ATTR Wifi::user_tcp_recon_cb(void *arg, sint8 err) {
    os_printf("Reconnect callback called, error code: %d !!! \r\n", err);
}

void ICACHE_FLASH_ATTR Wifi::user_dns_found(const char *name, ip_addr_t *ipaddr, void *arg) {
    struct espconn *pespconn = (struct espconn *)arg;
  
    if (ipaddr == NULL) {
        os_printf("user_dns_found NULL \r\n");
        return;
    }
  
    //dns got ip
    os_printf("user_dns_found %d.%d.%d.%d \r\n",
            *((uint8 *)&ipaddr->addr), *((uint8 *)&ipaddr->addr + 1),
            *((uint8 *)&ipaddr->addr + 2), *((uint8 *)&ipaddr->addr + 3));
  
    if (tcp_server_ip.addr == 0 && ipaddr->addr != 0) {
        // dns succeed, create tcp connection
        os_timer_disarm(&test_timer);
        tcp_server_ip.addr = ipaddr->addr;
        os_memcpy(pespconn->proto.tcp->remote_ip, &ipaddr->addr, 4); // remote ip of tcp server which get by dns
  
        pespconn->proto.tcp->remote_port = 80; // remote port of tcp server
        
        pespconn->proto.tcp->local_port = espconn_port(); //local port of ESP8266
  
        espconn_regist_connectcb(pespconn, user_tcp_connect_cb); // register connect callback
        espconn_regist_reconcb(pespconn, user_tcp_recon_cb); // register reconnect callback as error handler
  
        espconn_connect(pespconn); // tcp connect
    }
}

void ICACHE_FLASH_ATTR Wifi::user_dns_check_cb(void *arg) {
    os_printf("DNS CHECK CB CALLED\r\n");
    /*
    struct espconn *pespconn = (struct espconn *)arg;

    espconn_gethostbyname(pespconn, NET_DOMAIN, &tcp_server_ip, user_dns_found); // recall DNS function

    os_timer_arm(&test_timer, 1000, 0);
    */
}

void ICACHE_FLASH_ATTR Wifi::user_check_ip(void) {
    struct ip_info ipconfig;
  
    //disarm timer first
    os_timer_disarm(&test_timer);
  
    //get ip info of ESP8266 station
    wifi_get_ip_info(STATION_IF, &ipconfig);
  
    if (wifi_station_get_connect_status() == STATION_GOT_IP && ipconfig.ip.addr != 0) {
        os_printf("Connected to router and assigned IP!\r\n");
  
        // Connect to tcp server as NET_DOMAIN
        user_tcp_conn.proto.tcp = &user_tcp;
        user_tcp_conn.type = ESPCONN_TCP;
        user_tcp_conn.state = ESPCONN_NONE;
        
        /* Get IP of remote server */
        const char esp_tcp_server_ip[4] = {65, 19, 143, 6}; // remote IP of TCP server

        os_memcpy(user_tcp_conn.proto.tcp->remote_ip, esp_tcp_server_ip, 4);

        user_tcp_conn.proto.tcp->remote_port = 80;  // remote port

        user_tcp_conn.proto.tcp->local_port = espconn_port(); //local port of ESP8266

        espconn_regist_connectcb(&user_tcp_conn, user_tcp_connect_cb); // register connect callback
        espconn_regist_reconcb(&user_tcp_conn, user_tcp_recon_cb); // register reconnect callback as error handler
        espconn_connect(&user_tcp_conn); 
    } else {
        if ((wifi_station_get_connect_status() == STATION_WRONG_PASSWORD ||
                wifi_station_get_connect_status() == STATION_NO_AP_FOUND ||
                wifi_station_get_connect_status() == STATION_CONNECT_FAIL)) {
            os_printf("Connection to router failed!\r\n");
        } 
        else {
            //re-arm timer to check ip
            os_timer_setfn(&test_timer, (os_timer_func_t *)user_check_ip, NULL);
            os_timer_arm(&test_timer, 100, 0);
        }
    }
}
