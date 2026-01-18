#include <ESP32SPISlave.h> // Installer la bibliothèque ESP32SPISlave

ESP32SPISlave slave;

void setup() {
  Serial.begin(115200);
  slave.setDataMode(SPI_MODE0);
  slave.begin(); 
}

void loop() {
  String pressedKey = "5"; 
  uint8_t tx_buf[32] = {0};
  memcpy(tx_buf, pressedKey.c_str(), pressedKey.length());

  if (slave.available()) {
    slave.pushString(pressedKey);
    slave.wait();
  }
}
