#include <ESP32SPISlave.h>
#include <U8g2lib.h> // Exemple pour un petit écran OLED sur le module

ESP32SPISlave slave;
const int potPin = 2; // Pin analogique du potentiomètre

void setup() {
  slave.setDataMode(SPI_MODE0);
  slave.begin();
}

void loop() {
  int potValue = analogRead(potPin);
  String valStr = String(map(potValue, 0, 4095, 0, 100)); // Volume 0-100

  if (slave.available()) {
    slave.pushString(valStr);
    slave.wait();
  }
  
  // Code pour l'écran local du module ici...
}