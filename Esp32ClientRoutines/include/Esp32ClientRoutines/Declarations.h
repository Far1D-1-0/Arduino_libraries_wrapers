#ifndef ESP32CLIENTROUTINES_DECLARATIONS_H
#define ESP32CLIENTROUTINES_DECLARATIONS_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi_types.h>
#include <IPAddress.h>

namespace Esp32ClientRoutines {
    extern const int WIFI_SEARCH_RETRY_LIMIT;
    extern const int WIFI_CONNECT_RETRY_LIMIT;
    extern const int WIFI_RECONNECT_RETRY_LIMIT;
}

#endif //ESP32CLIENTROUTINES_DECLARATIONS_H