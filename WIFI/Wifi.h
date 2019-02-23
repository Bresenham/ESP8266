#ifndef WIFI_H
#define WIFI_H

class Wifi {
    public:
        void connect(void);
        void post_request(char *data);
        Wifi();
    private:
        const char *SSID;
        const char *PW;

};

#endif