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

#define REQUEST_TEXT "GET / HTTP/1.1\r\nUser-Agent: curl/7.37.0\r\nHost: %s\r\nAccept: */*\r\n\r\n"
#define NET_DOMAIN  "scalar.heliohost.org"
#define PACKET_SIZE (2 * 1024)

LOCAL os_timer_t test_timer;
LOCAL struct espconn user_tcp_conn;
LOCAL struct _esp_tcp user_tcp;
ip_addr_t tcp_server_ip;

void ICACHE_FLASH_ATTR Wifi::connect(void) {
    struct station_config stationConf;
    
    stationConf.bssid_set = 0;
    os_memcpy(&stationConf.ssid, SSID, 32);
    os_memcpy(&stationConf.password, PW, 64);

    wifi_station_set_config(&stationConf);
    wifi_station_connect();
}

Wifi::Wifi() {
    wifi_set_opmode(STATION_MODE);
    this->SSID = "";
    this->PW = "";
}

static void ICACHE_FLASH_ATTR user_tcp_recv_cb(void *arg, char *pusrdata, unsigned short length) {
    os_printf("Received data string: %s \r\n", pusrdata);
}

static void ICACHE_FLASH_ATTR user_tcp_sent_cb(void *arg) {
    os_printf("Sent callback: data sent successfully.\r\n");
}

static void ICACHE_FLASH_ATTR user_tcp_discon_cb(void *arg) {
    os_printf("Disconnected from server.\r\n");
}

static void ICACHE_FLASH_ATTR user_send_data(espconn *pespconn) {
    char *pbuf = (char *)os_zalloc(PACKET_SIZE);
  
    os_sprintf(pbuf, REQUEST_TEXT, NET_DOMAIN);
  
    espconn_send(pespconn, (uint8_t *)pbuf, os_strlen(pbuf));
     
    os_free(pbuf);
}

static void ICACHE_FLASH_ATTR user_tcp_connect_cb(void *arg) {
    struct espconn *pespconn = (struct espconn *)arg;
  
    os_printf("Connected to server...\r\n");
  
    espconn_regist_recvcb(pespconn, user_tcp_recv_cb);
    espconn_regist_sentcb(pespconn, user_tcp_sent_cb);
    espconn_regist_disconcb(pespconn, user_tcp_discon_cb);
     
    user_send_data(pespconn);
}

static void ICACHE_FLASH_ATTR user_tcp_recon_cb(void *arg, sint8 err) {
    os_printf("Reconnect callback called, error code: %d !!! \r\n", err);
}

static void ICACHE_FLASH_ATTR user_dns_found(const char *name, ip_addr_t *ipaddr, void *arg) {
    struct espconn *pespconn = (struct espconn *)arg;
  
    if (ipaddr == NULL) 
    {
        os_printf("user_dns_found NULL \r\n");
        return;
    }
  
    //dns got ip
    os_printf("user_dns_found %d.%d.%d.%d \r\n",
            *((uint8 *)&ipaddr->addr), *((uint8 *)&ipaddr->addr + 1),
            *((uint8 *)&ipaddr->addr + 2), *((uint8 *)&ipaddr->addr + 3));
  
    if (tcp_server_ip.addr == 0 && ipaddr->addr != 0) 
    {
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

static void ICACHE_FLASH_ATTR user_dns_check_cb(void *arg) {
    struct espconn *pespconn = (struct espconn *)arg;

    espconn_gethostbyname(pespconn, NET_DOMAIN, &tcp_server_ip, user_dns_found); // recall DNS function

    os_timer_arm(&test_timer, 1000, 0);
}

static void ICACHE_FLASH_ATTR user_check_ip(void) {
    struct ip_info ipconfig;
  
    //disarm timer first
    os_timer_disarm(&test_timer);
  
    //get ip info of ESP8266 station
    wifi_get_ip_info(STATION_IF, &ipconfig);
  
    if (wifi_station_get_connect_status() == STATION_GOT_IP && ipconfig.ip.addr != 0) 
    {
        os_printf("Connected to router and assigned IP!\r\n");
  
        // Connect to tcp server as NET_DOMAIN
        user_tcp_conn.proto.tcp = &user_tcp;
        user_tcp_conn.type = ESPCONN_TCP;
        user_tcp_conn.state = ESPCONN_NONE;
        
        /* Get IP of remote server */
        tcp_server_ip.addr = 0;
        espconn_gethostbyname(&user_tcp_conn, NET_DOMAIN, &tcp_server_ip, user_dns_found); // DNS function

        os_timer_setfn(&test_timer, (os_timer_func_t *)user_dns_check_cb, &user_tcp_conn);
        os_timer_arm(&test_timer, 1000, 0);
    } 
    else {
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
