#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <secrets.h>


//Provide the token generation process info.
#include "addons/TokenHelper.h"

//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Defines the Digital Pin of the "On Board LED".
#define On_Board_LED 2


//Define Firebase Data object.
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

//======================================== Millis variable to read data from firebase database.
unsigned long sendDataPrevMillis = 0;
const long intervalMillis = 10000; //--> Read data from firebase database every 10 seconds.
//======================================== 

// Boolean variable for sign in status.
bool signupOK = false;

float read_random_Float_Val;
int read_random_Int_Val;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  Serial.println();

  pinMode(On_Board_LED, OUTPUT);

  //---------------------------------------- The process of connecting the WiFi on the ESP32 to the WiFi Router/Hotspot.
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("---------------Connection");
  Serial.print("Connecting to : ");
  Serial.println(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");

    digitalWrite(On_Board_LED, HIGH);
    delay(250);
    digitalWrite(On_Board_LED, LOW);
    delay(250);
  }
  digitalWrite(On_Board_LED, LOW);
  Serial.println();
  Serial.print("Successfully connected to : ");
  Serial.println(WIFI_SSID);
  //Serial.print("IP : ");
  //Serial.println(WiFi.localIP());
  Serial.println("---------------");
  //---------------------------------------- 

  // Assign the api key (required).
  config.api_key = API_KEY;

  // Assign the RTDB URL (required).
  config.database_url = DATABASE_URL;

  /* Assign the user sign in credentials */
  auth.user.email = FIREBASE_USER;
  auth.user.password = FIREBASE_PASSWORD;

  config.token_status_callback = tokenStatusCallback; //--> see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}


void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > intervalMillis || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    //---------------------------------------- Generate random values.
    int randNumber = random(15, 40);
    float f = (float)randNumber / 1.01;
    int i = (int(f*100));
    float store_random_Float_Val = float(i) / 100;
    int store_random_Int_Val = random(10, 99);
    //---------------------------------------- 

    //---------------------------------------- 
    Serial.println();
    Serial.println("---------------Random Value");
    Serial.print("Random Float_Val : ");
    Serial.println(store_random_Float_Val);
    Serial.print("Random Int_Val   : ");
    Serial.println(store_random_Int_Val);
    Serial.println("---------------");
    //---------------------------------------- 

    //---------------------------------------- The process of sending/storing data to the firebase database.
    Serial.println();
    Serial.println("---------------Store Data");
    digitalWrite(On_Board_LED, HIGH);
    
    // Write an Int number on the database path test/random_Float_Val.
    if (Firebase.RTDB.setFloat(&fbdo, "/air_conditioning/float", store_random_Float_Val)) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    // Write an Float number on the database path test/random_Int_Val.
    if (Firebase.RTDB.setInt(&fbdo, "/air_conditioning/int", store_random_Int_Val)) {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    digitalWrite(On_Board_LED, LOW);
    Serial.println("---------------");
    //---------------------------------------- 
  }
}