#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ArduinoJson.h>

#define RETRY_INTERVAL 2000


bool validateJson(const char* input) {
  StaticJsonDocument<0> doc, filter;
  return deserializeJson(doc, input, DeserializationOption::Filter(filter)) == DeserializationError::Ok;
}

void receiveCallBackFunction(uint8_t *senderMac, uint8_t *incomingData, uint8_t len) 
{
  //if ( validateJson((char*) incomingData) ) 
  {  
    Serial.println( (char*) incomingData );
    uint8_t msg[] = "OK";
    uint8_t msg_len = sizeof(msg);
    esp_now_send(senderMac, msg, msg_len);
  }
}


void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);

#ifdef VERBOSE
  Serial.println("ESP-Now Receiver");
  Serial.printf("Transmitter mac: %s\n", WiFi.macAddress().c_str());
#endif

  if (esp_now_init() != 0) 
  {
#ifdef VERBOSE   
    Serial.println("ESP_Now init failed...");
#endif    
    delay(RETRY_INTERVAL);
    ESP.restart();
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(receiveCallBackFunction);
#ifdef VERBOSE
  Serial.println("MASTER ready. Waiting for messages...");
#endif

}

void loop() {

}

