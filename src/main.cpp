#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ArduinoJson.h>

#define RETRY_INTERVAL 2000

StaticJsonDocument<1024> doc;


bool validateJson(const char* input) 
{
  StaticJsonDocument<0> doc, filter;
  return deserializeJson(doc, input, DeserializationOption::Filter(filter)) == DeserializationError::Ok;
}

//wait for a message to come in from the setial port and return it
String receiveSerialMessage()
 {
  String message = "";
  while (Serial.available()) 
  {
    // read the incoming byte:
    char incomingByte = Serial.read();
    message += incomingByte;
  }
  return message;
}

void receiveCallBackFunction(uint8_t *senderMac, uint8_t *incomingData, uint8_t len) 
{
  if ( validateJson((char*) incomingData) ) 
  {  
    Serial.println( (char*) incomingData );

    String response = receiveSerialMessage();

    DeserializationError error = deserializeJson(doc, response);

    if ( error ) // no data
    {
      uint8_t msg[] = "{}";
      uint8_t msg_len = sizeof(msg);
      esp_now_send(senderMac, msg, msg_len);
    }
    else
    {
      esp_now_send (senderMac, (uint8_t*) response.c_str(), response.length());
    }
  }
}


void setup()
{
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

