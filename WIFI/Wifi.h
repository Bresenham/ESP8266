#ifndef WIFI_H
#define WIFI_H

class Wifi {
    public:
        void connect(void);
        void post_request(char *data);
        Wifi();
    private:
        static void user_tcp_recv_cb(void *arg, char *pusrdata, unsigned short length);
        static void user_tcp_sent_cb(void *arg);
        static void user_tcp_discon_cb(void *arg);
        static void user_send_data(struct espconn *pespconn);
        static void user_tcp_connect_cb(void *arg);
        static void user_tcp_recon_cb(void *arg, sint8 err);
        static void user_dns_found(const char *name, ip_addr_t *ipaddr, void *arg);
        static void user_dns_check_cb(void *arg);
        static void user_check_ip(void);

        const char *SSID;
        const char *PW;

};

#endif
