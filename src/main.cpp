#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>


#include "Secrets.h"
#include "wifi/WiFiConnection.h"

#include "TinyIRSender.hpp"

#define IR_SEND_PIN 4


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;


unsigned long sendDataPrevMillis = 0;
const long intervalMillis = 20000; 

float read_random_Float_Val;
int read_random_Int_Val;

void setup() {
    Serial.begin(115200);
    Serial.println();

    pinMode(ON_BOARD_LED, OUTPUT);

    connectWiFi();

    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    config.token_status_callback = tokenStatusCallback;

    auth.user.email = FIREBASE_USER;
    auth.user.password = FIREBASE_PASSWORD;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

uint16_t sAddress = 0x707;
uint16_t sMuteCommand = 0xF00F;
uint16_t sRepeats = 0;


void loop() {
    if (Firebase.ready() && (millis() - sendDataPrevMillis > intervalMillis || sendDataPrevMillis == 0)){
        sendDataPrevMillis = millis();

        Serial.println();
        Serial.println("---------------Store Data");
        digitalWrite(ON_BOARD_LED, HIGH);

        if (Firebase.RTDB.getFloat(&fbdo, "/air_conditioning/int")) {
            if (fbdo.dataType() == "int") {
                read_random_Int_Val = fbdo.intData();
                Serial.println("PATH: " + fbdo.dataPath());
                Serial.println("TYPE: " + fbdo.dataType());
                Serial.print("Random  Int_Val: ");
                Serial.println(read_random_Int_Val);
            }

            if(read_random_Int_Val == 2) {
                Serial.print(F(" command=0x"));
                Serial.print(sMuteCommand, HEX);
                Serial.print(F(" repeats="));
                Serial.print(sRepeats);
                Serial.println();

                Serial.println(F("Send NEC with 8 bit address"));
                Serial.flush();
                sendNEC(IR_SEND_PIN, sAddress, sMuteCommand, sRepeats);

                delay(1000);
            }

            read_random_Int_Val = 0;
        }
        else {
            Serial.println(fbdo.errorReason());
        }

        digitalWrite(ON_BOARD_LED, LOW);
        Serial.println("---------------");
    }
}