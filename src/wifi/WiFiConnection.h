#ifndef HEADER_FILE_NAME
#define HEADER_FILE_NAME

    #include <WiFi.h>
    #include "Secrets.h"

    static void connectWiFi() {
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        Serial.println("---------------Connection");
        Serial.print("Connecting to : ");
        Serial.println(WIFI_SSID);
        while (WiFi.status() != WL_CONNECTED){
            Serial.print(".");

            digitalWrite(ON_BOARD_LED, HIGH);
            delay(250);
            digitalWrite(ON_BOARD_LED, LOW);
            delay(250);
        }
        digitalWrite(ON_BOARD_LED, LOW);
        Serial.println();
        Serial.print("Successfully connected to : ");
        Serial.println(WIFI_SSID);
        Serial.println("---------------");
    }

#endif

