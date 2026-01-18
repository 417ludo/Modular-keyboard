#include <BleKeyboard.h>
#include <SPI.h>
#include <TFT_eSPI.h>

// --- CONFIGURATION CLAVIER ---
BleKeyboard bleKeyboard("Clavier Hybride Pro", "Maker", 100);

// Pins Matrice Mécanique
const int latchPin = 5; 
const int clockPin = 18; 
const int dataPin = 23;  

// Définition des Lignes (Exemple : 5 lignes)
const int rowPins[5] = {13, 12, 14, 27, 26}; 
const int numRows = 5;
const int numCols = 16; 

// Touches Magnétiques
const int hallPins[4] = {32, 33, 34, 35};
bool hallState[4] = {false, false, false, false};

// SPI pour Modules Externes (C3)
#define MY_SCK  18 
#define MY_MISO 19
#define MY_MOSI 23
#define CS_MODULE 15 

TFT_eSPI tft = TFT_eSPI();

char keyMap[5][16] = {
  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'}
};

bool lastKeyState[5][16] = {false};

void setup() {
  Serial.begin(115200);
  bleKeyboard.begin();

  // Setup 595
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for(int i=0; i<numRows; i++) {
    pinMode(rowPins[i], INPUT_PULLDOWN);
  }

  // Écran
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Clavier BT Prêt", 10, 10);
}

// --- LOGIQUE MATRICE MÉCANIQUE ---
void scanMechanicalMatrix() {
  for (int c = 0; c < numCols; c++) {
    uint16_t columnMask = (1 << c);
    
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (columnMask >> 8));   // Deuxième 595
    shiftOut(dataPin, clockPin, MSBFIRST, (columnMask & 0xFF)); // Premier 595
    digitalWrite(latchPin, HIGH);
    delayMicroseconds(10);

    for (int r = 0; r < numRows; r++) {
      bool pressed = digitalRead(rowPins[r]);
      
      if (pressed && !lastKeyState[r][c]) {
        bleKeyboard.press(keyMap[r][c]);
        lastKeyState[r][c] = true;
      } 
      else if (!pressed && lastKeyState[r][c]) {
        bleKeyboard.release(keyMap[r][c]);
        lastKeyState[r][c] = false;
      }
    }
  }
}

// --- LOGIQUE TOUCHES MAGNÉTIQUES ---
void scanMagneticKeys() {
  for(int i = 0; i < 4; i++) {
    int val = analogRead(hallPins[i]);
    if(val > 2800 && !hallState[i]) {
      bleKeyboard.press('0' + i);
      hallState[i] = true;
    } else if (val < 2200 && hallState[i]) {
      bleKeyboard.release('0' + i);
      hallState[i] = false;
    }
  }
}

// --- LOGIQUE MODULES SPI (C3) ---
void checkSPIModule() {
  digitalWrite(CS_MODULE, LOW);
  uint8_t received = SPI.transfer(0x00); 
  digitalWrite(CS_MODULE, HIGH);

  if (received != 0) {
    bleKeyboard.write(received);
  }
}

void loop() {
  if (bleKeyboard.isConnected()) {
    scanMechanicalMatrix();
    scanMagneticKeys();
    checkSPIModule();
  
}
