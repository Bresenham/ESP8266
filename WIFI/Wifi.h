#ifndef WIFI_H
#define WIFI_H

class Wifi {
    public:
        void send_data(char *data);
        void scan_wifi_networks(void);
        static void register_could_not_connect_cb(void (*callback)(void));
        static void register_connected_cb(void (*callback)(void));
        Wifi();
    private:
        void connect(void);
        static void user_tcp_recv_cb(void *arg, char *pusrdata, unsigned short length);
        static void user_tcp_sent_cb(void *arg);
        static void user_tcp_discon_cb(void *arg);
        static void user_send_data(struct espconn *pespconn);
        static void user_tcp_connect_cb(void *arg);
        static void user_tcp_recon_cb(void *arg, sint8 err);
        static void user_dns_found(const char *name, ip_addr_t *ipaddr, void *arg);
        static void user_dns_check_cb(void *arg);
        static void user_check_ip(void);
        static void scan_done_cb(void *arg, STATUS status);

        const char *SSID;
        const char *PW;

        static char data_to_send[12];

        static void (*not_connected_cb)();
        static void (*connected_cb)();

};

#endif
