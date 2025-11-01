#ifndef ESP32CLIENTROUTINES_TCPCLIENTROUTINES_H
#define ESP32CLIENTROUTINES_TCPCLIENTROUTINES_H

#include <Esp32ClientRoutines/Declarations.h>

namespace Esp32ClientRoutines {
    class TCPClientRoutines {
    public:
        static void set_values(const IPAddress& ip, uint16_t port, bool serial_available = false, unsigned long _interval = 10000);
        static void connect();
        static bool send(const uint8_t* data, size_t length);
        static bool read(uint8_t* buffer, size_t length);
        static void stop();

    private:
        static bool serialIsAvailable;
        static IPAddress server_ip;
        static uint16_t server_port;
        // static unsigned long prev;
        static unsigned long interval;
        static WiFiClient client;
    };
} // Esp32ClientRoutines

#endif //ESP32CLIENTROUTINES_TCPCLIENTROUTINES_H