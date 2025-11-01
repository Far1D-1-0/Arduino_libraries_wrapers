#ifndef ESP32CLIENTROUTINES_WIFIROUTINES_H
#define ESP32CLIENTROUTINES_WIFIROUTINES_H

#include <Esp32ClientRoutines/Declarations.h>

namespace Esp32ClientRoutines {
    class WiFiRoutines {
    public:
        static void set_values(const char* ssid_, const char* password_,
                               bool canReset_, bool serialIsAvailable_);
        static void set_network_credentials(const char* ssid_, const char* password_);
        static void set_reset_flg(bool canReset_);
        static void set_serial_flg(bool serialIsAvailable_);
        static void scan();
        static bool search();
        static void connect();
        static void check();

        static wifi_mode_t get_wifi_mode();
        static IPAddress get_local_ip();
        static void print_device_info();

    private:
        static wifi_mode_t wifi_mode;
        static const char *ssid;
        static const char* password;
        static IPAddress local_ip;
        static bool canReset;
        static bool serialIsAvailable;
    };
} // Esp32ClientRoutines

#endif //ESP32CLIENTROUTINES_WIFIROUTINES_H