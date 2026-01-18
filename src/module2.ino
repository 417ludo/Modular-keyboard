#include <ESP32SPISlave.h>
#include <U8g2lib.h>

ESP32SPISlave slave;
const int potPin = 2;
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
  
}
