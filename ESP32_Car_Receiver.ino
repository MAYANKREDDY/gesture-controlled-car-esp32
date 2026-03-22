#include <WiFi.h>
#include <esp_now.h>

// Motor driver pins
#define IN1 26   // Motor A input 1
#define IN2 27   // Motor A input 2
#define IN3 12   // Motor B input 1
#define IN4 13   // Motor B input 2

String command = "";

// ====== MOTOR CONTROL ======
void moveCar(const String &cmd) {
  String c = cmd;
  c.toUpperCase();

  if (c == "FORWARD") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else if (c == "BACKWARD") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else if (c == "LEFT") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else if (c == "RIGHT") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else { // STOP
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}

// ====== ESP-NOW RECEIVE CALLBACK ======
void onRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  char buffer[50];
  memcpy(buffer, data, len);
  buffer[len] = '\0';

  String recv = String(buffer);
  recv.trim();

  if (recv.length() == 0) return;

  Serial.print("Command Received: ");
  Serial.println(recv);

  moveCar(recv);
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW INIT FAILED!");
    return;
  }

  esp_now_register_recv_cb(onRecv);

  Serial.println("Receiver Ready");
}

void loop() {}
