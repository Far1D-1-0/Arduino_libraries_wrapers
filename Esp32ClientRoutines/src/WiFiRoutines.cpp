#include <Esp32ClientRoutines/WiFiRoutines.h>

namespace Esp32ClientRoutines {
    //Define Static Members
    wifi_mode_t WiFiRoutines::wifi_mode = WIFI_STA;
    const char* WiFiRoutines::ssid = nullptr;
    const char* WiFiRoutines::password = nullptr;
    IPAddress WiFiRoutines::local_ip; // defaults to 0.0.0.0
    bool WiFiRoutines::canReset = false;
    bool WiFiRoutines::serialIsAvailable = false;


    void WiFiRoutines::set_values(const char *ssid_, const char *password_, const bool canReset_, const bool serialIsAvailable_) {
        ssid = ssid_;
        password = password_;
        canReset = canReset_;
        serialIsAvailable = serialIsAvailable_;

        WiFi.mode(wifi_mode);
        WiFi.disconnect();
    }

    void WiFiRoutines::set_network_credentials(const char *ssid_, const char *password_) {
        ssid = ssid_;
        password = password_;

        WiFi.mode(wifi_mode);
        WiFi.disconnect();
    }

    void WiFiRoutines::set_reset_flg(const bool canReset_) {
        canReset = canReset_;
    }

    void WiFiRoutines::set_serial_flg(const bool serialIsAvailable_) {
        serialIsAvailable = serialIsAvailable_;
    }

    void WiFiRoutines::scan() {
        if (serialIsAvailable) {
            Serial.println("Scanning for available networks...");
            int n = WiFi.scanNetworks();
            if (n <= 0) {
                Serial.println(n == 0 ? "No networks found." : "Scan failed.");
                return;
            }
            Serial.println("Scan complete.");
            Serial.print(n);
            Serial.println(" networks found:");
            for (int i = 0; i < n; ++i) {
                Serial.print(i + 1);
                Serial.print(". SSID: ");
                Serial.print(WiFi.SSID(i));
                Serial.print(" | RSSI: ");
                Serial.print(WiFi.RSSI(i));
                Serial.print(" dBm | Encryption: ");
                Serial.print(WiFi.encryptionType(i));
                Serial.println();
            }
        }
    }

    bool WiFiRoutines::search() {
        if (serialIsAvailable) Serial.printf("Searching for SSID: %s\n", ssid);

        int n = WiFi.scanNetworks();
        if (n <= 0) {
            if (serialIsAvailable) Serial.println(n == 0 ? "No networks found." : "Scan failed.");
            return false;
        }

        for (int i = 0; i < n; ++i) {
            if (strcmp(WiFi.SSID(i).c_str(), ssid) == 0) {
                if (serialIsAvailable) Serial.println("SSID found.");
                WiFi.scanDelete(); // free memory
                return true;
            }
        }

        if (serialIsAvailable) Serial.println("SSID not found.");
        WiFi.scanDelete();
        return false;
    }

    void WiFiRoutines::connect() {
        int n = 0;
        while (!search()) {
            n++;
            if (serialIsAvailable) Serial.println("Retrying...");
            delay(1000); // Wait before retrying
            if (n >= WIFI_SEARCH_RETRY_LIMIT && canReset) {
                if (serialIsAvailable) Serial.println("SSID not found after multiple attempts. Restarting...");
                ESP.restart();
            }
        }
        n = 0;
        if (serialIsAvailable) Serial.println("Connecting...");
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            n++;
            Serial.print('.');
            delay(500);
            if (n >= WIFI_CONNECT_RETRY_LIMIT && canReset) {
                if (serialIsAvailable) Serial.println("\nConnection failed after multiple attempts. Restarting...");
                ESP.restart();
            }
        }
        if (serialIsAvailable) {
            Serial.println("\nConnected to WiFi.");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
            local_ip = WiFi.localIP();
        }
    }

    void WiFiRoutines::check() {
        if (WiFi.status() != WL_CONNECTED) {
            int n = 0;
            if (serialIsAvailable) Serial.println("WiFi disconnected. Attempting to reconnect...");
            WiFi.disconnect(); delay(100);
            WiFi.reconnect();

            while (WiFi.status() != WL_CONNECTED) {
                n++;
                if (serialIsAvailable) Serial.print('.');
                delay(500);
                if (n >= WIFI_RECONNECT_RETRY_LIMIT && canReset) {
                    if (serialIsAvailable) Serial.println("\nReconnection failed after multiple attempts. Restarting...");
                    ESP.restart();
                } else if (n >= 20 && !canReset) {
                    if (serialIsAvailable) Serial.println("\nReconnection failed after multiple attempts.");
                    return;
                }
            }
            if (serialIsAvailable) {
                Serial.println("\nReconnected to WiFi.");
                Serial.print("IP Address: ");
                Serial.println(WiFi.localIP());
                local_ip = WiFi.localIP();
            }
        }
    }

    wifi_mode_t WiFiRoutines::get_wifi_mode() {
        return wifi_mode;
    }

    IPAddress WiFiRoutines::get_local_ip() {
        return local_ip;
    }

    void WiFiRoutines::print_device_info() {
    }
}

