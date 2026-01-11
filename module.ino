#include <ESP32SPISlave.h> // Installer la bibliothèque ESP32SPISlave

ESP32SPISlave slave;

void setup() {
  Serial.begin(115200);
  // Sur le C3, les pins SPI par défaut sont : SCK: 4, MISO: 5, MOSI: 6, SS: 7
  slave.setDataMode(SPI_MODE0);
  slave.begin(); 
}

void loop() {
  String pressedKey = "5"; // Simulation : touche 5 pressée
  uint8_t tx_buf[32] = {0};
  memcpy(tx_buf, pressedKey.c_str(), pressedKey.length());

  // Attend que le maître demande les données
  if (slave.available()) {
    slave.pushString(pressedKey);
    slave.wait();
  }
}