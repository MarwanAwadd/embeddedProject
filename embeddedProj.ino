#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>

#define WIFI_SSID "Bedazzle Razzle Williams"
#define WIFI_PASSWORD "m2112316!"

#define API_KEY "AIzaSyAqoBHlc242mhSSuT8K5BN9uHRrD5bSRVI"
#define FIREBASE_PROJECT_ID "embedded-parkingsystem"
#define USER_EMAIL "marwanawad@aucegypt.edu"
#define USER_PASSWORD "m2112316!"

const int IRpin1 = 15;
const int IRpin2 = 4;
const int IRpin3 = 2;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;
bool space1 = true;
bool space1Change = false;
bool space2 = true;
bool space2Change = false;
bool space3 = true;
bool space3Change = false;


void FirestoreUpdate(String documentPath, bool s){
    FirebaseJson content;

    content.clear();
    content.set("fields/Available/booleanValue", s);
    Serial.print("Update a document... ");

    if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), "Available"))
        Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    else
        Serial.println(fbdo.errorReason());
}

void setup()
{
    Serial.begin(9600);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.token_status_callback = tokenStatusCallback;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    pinMode(IRpin1, INPUT);
    pinMode(IRpin2, INPUT);
    pinMode(IRpin3, INPUT);

}

void loop()
{

  if(digitalRead(IRpin1) == LOW){
    space1 = false;
  }
  else{
    space1 = true;
  }

  if(digitalRead(IRpin2) == LOW){
    space2 = false;
  }
  else{
    space2 = true;
  }

  if(digitalRead(IRpin3) == LOW){
    space3 = false;
  }
  else{
    space3 = true;
  }

Serial.println(space3);  

    if (Firebase.ready() && (millis() - dataMillis > 10000 || dataMillis == 0))
    {
        dataMillis = millis();

        String documentPathSpace1 = "spaces/NMGXGWogVFusVTBV7ADA";
        String documentPathSpace2 = "spaces/fId9ArR0nwVWkG4e0max";
        String documentPathSpace3 = "spaces/jjq9F878NbrNKMfuyMtA";

        FirestoreUpdate(documentPathSpace1, space1);
        FirestoreUpdate(documentPathSpace2, space2);
        FirestoreUpdate(documentPathSpace3, true);

        
    }
}
