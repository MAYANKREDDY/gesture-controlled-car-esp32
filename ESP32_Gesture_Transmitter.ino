#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

typedef struct {
  char command[15];
} struct_message;

struct_message myData;

// Receiver MAC
uint8_t receiverMAC[] = {0x0C, 0xB8, 0x15, 0x78, 0xC7, 0xd0};

void onSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  delay(200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (!mpu.begin()) {
    Serial.println("MPU6050 NOT FOUND!");
    while (1) delay(10);
  }

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW INIT FAILED");
    return;
  }

  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer!");
  }
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  if (a.acceleration.x > 5) strcpy(myData.command, "FORWARD");
  else if (a.acceleration.x < -5) strcpy(myData.command, "BACKWARD");
  else if (a.acceleration.y > 5) strcpy(myData.command, "LEFT");
  else if (a.acceleration.y < -5) strcpy(myData.command, "RIGHT");
  else strcpy(myData.command, "STOP");

  esp_now_send(receiverMAC, (uint8_t*)&myData, sizeof(myData));

  delay(150);
}
