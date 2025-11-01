#include <Esp32ClientRoutines/TCPClientRoutines.h>

namespace Esp32ClientRoutines {
    bool TCPClientRoutines::serialIsAvailable = false;
    IPAddress TCPClientRoutines::server_ip = IPAddress(0, 0, 0, 0);
    uint16_t TCPClientRoutines::server_port = 0;
    // unsigned long prev = 0;
    unsigned long TCPClientRoutines::interval = 10000;
    WiFiClient TCPClientRoutines::client;


    void TCPClientRoutines::set_values(const IPAddress& ip, const uint16_t port, const bool serial_available, const unsigned long _interval) {
        server_ip = ip;
        server_port = port;
        serialIsAvailable = serial_available;
        interval = _interval;
    }

    void TCPClientRoutines::connect() {
        if (serialIsAvailable) Serial.printf("Connecting to TCP server: %s:%d\n", server_ip.toString().c_str(), server_port);
        while (!client.connect(server_ip, server_port)) {
            Serial.print('.');
            delay(500);
        } Serial.println("\nConnected!");
    }

    bool TCPClientRoutines::send(const uint8_t *data, const size_t length) {
        if (serialIsAvailable) Serial.printf("Data send | Bytes: %d\n", length);
        size_t sent = 0;
        unsigned long prev = millis();
        while (sent < length) {
            const size_t n = client.write(data + sent, length - sent);
            if (millis() - prev >= interval && n == 0) {
                if (serialIsAvailable) Serial.println("Send timeout");
                return false;
            }
            if (millis() - prev >= interval) {
                if (serialIsAvailable) Serial.println("Connection slow");
                prev = millis();
            }
            sent += n;
            if (serialIsAvailable) Serial.printf("Bytes pending: %d\n", length - sent);
        }
        return true;
    }

    bool TCPClientRoutines::read(uint8_t *buffer, size_t length) {
        Serial.printf("Received Bytes | Bytes: %d\n", length);
        size_t got = 0;
        unsigned long prev = millis();
        while (got < length) {
            const size_t n = client.read(buffer + got, length - got);
            if (millis() - prev >= interval && n == 0) {
                if (serialIsAvailable) Serial.println("Read timeout");
                return false;
            }
            if (millis() - prev >= interval) {
                if (serialIsAvailable) Serial.println("Connection slow");
                prev = millis();
            }
            got += n;
            if (serialIsAvailable) Serial.printf("Bytes pending: %d\n", length - got);
        }
        return true;
    }

    void TCPClientRoutines::stop() {
        client.stop();
        if (serialIsAvailable) Serial.println("Client stopped");
    }
}
