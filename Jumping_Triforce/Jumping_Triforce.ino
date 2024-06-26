// include libraries
#include <Arduino_GFX_Library.h>

#include "image.h"

// Pin definitions
#define BTN 41

//display definitions
#define GFX_DEV_DEVICE ARDUINO_M5Stack_ATOMS3
#define GFX_BL 16
Arduino_DataBus *bus = new Arduino_ESP32SPI(33 /* DC */, 15 /* CS */, 17 /* SCK */, 21 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_GC9107(bus, 34 /* RST */, 0 /* rotation */, true /* IPS */);

// State definitions
#define TRUE 1
#define FALSE 0

// variables
unsigned int pixel;

void setup() {
  // Serial Comunication
  Serial.begin(115200);

  // Display initialization
  if (!gfx->begin())
  // if (!gfx->begin(80000000)) /* specify data bus speed */
  {
    Serial.println("gfx->begin() failed!");
  }
  #ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
  #endif
  gfx->fillScreen(BLACK);

  // Pin Modes
  pinMode(BTN, INPUT);
}

void loop() {
  while(pixel <= epd_bitmap_array_LEN) {
    for(int x; x <= 128; x++) {
      for(int y; y <= 128; y++) {
        gfx->writePixel(x, y, epd_bitmap_frame1[pixel]);
        pixel++;
      }
    }
  }
  pixel = 0;
}