#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6JMfG8zkN"
#define BLYNK_TEMPLATE_NAME "Rain drop sensor"
#define BLYNK_AUTH_TOKEN "zE5lWXwSKfc6YcngfXRZ9ZJfEnucEGPl"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// WiFi
char ssid[] = "pop";
char pass[] = "pianjing";

// Interval
unsigned long interval = 0;

// Servo
Servo myServo;

// Sensor & Servo
int sensorPin = 34;
int servoPin = 5;
int threshold = 3500;

bool isRaining = false;

// Status untuk dashboard
String statusAtap = "Terbuka";

void setup() {
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  myServo.attach(servoPin);
  myServo.write(0);

  // Kirim data awal ke Blynk
  Blynk.virtualWrite(V0, statusAtap);
  Blynk.virtualWrite(V34, 0);
}

void loop() {
  Blynk.run();

  if (millis() - interval > 1000) {
    interval = millis();

    int sensorValue = analogRead(sensorPin);

    Serial.print("Sensor Analog: ");
    Serial.println(sensorValue);

    // V34 = nilai sensor
    Blynk.virtualWrite(V34, sensorValue);

    // Saat hujan
    if (sensorValue < threshold && !isRaining) {
      Serial.println("Hujan -> Tutup");
      myServo.write(90);
      isRaining = true;

      statusAtap = "Hujan";
      Blynk.virtualWrite(V0, statusAtap);
    }

    // Saat tidak hujan
    if (sensorValue >= threshold && isRaining) {
      Serial.println("Tidak hujan -> Buka");
      myServo.write(0);
      isRaining = false;

      statusAtap = "Cerah";
      Blynk.virtualWrite(V0, statusAtap);
    }
  }
}