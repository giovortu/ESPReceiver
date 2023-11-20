#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#define RETRY_INTERVAL 5000

uint8_t mac[] = {0x82, 0x88, 0x88, 0x88, 0x88, 0x88};

//static uint8_t PEER[] = {0x02, 0x00, 0x00, 0x45, 0x53, 0x50};
static uint8_t broadcastAddress[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};


void receiveCallBackFunction(uint8_t *senderMac, uint8_t *incomingData, uint8_t len) 
{
  Serial.println( (char*) incomingData );
  uint8_t msg[] = "OK";
  uint8_t msg_len = sizeof(msg);
  esp_now_send(senderMac, msg, msg_len);
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
   
    Serial.println("ESP_Now init failed...");
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

