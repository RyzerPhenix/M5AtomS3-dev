// include libraries
#include <Arduino_GFX_Library.h>

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

// variable definitions
char r, g, b;
bool RtoG, GtoB, BtoR;
bool buttonState, buttonPressed;
bool cycleFinished;

// function prototype
void cycleRGB();
void printInfo();
void button();

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

  // Set start Colour
  r = 0xff;
  g = 0x00;
  b = 0x00;

  // Initialize States
  RtoG = TRUE;
  GtoB = FALSE;
  BtoR = FALSE;
  cycleFinished = FALSE;
  buttonState = 0;
  buttonPressed = FALSE;
}

void loop() {
  button();
  printInfo();
}

void cycleRGB() {
  if(RtoG){
    if(r <= 0x01 && g >= 0xfe){
      RtoG = FALSE;
      GtoB = TRUE;
    }
    r--;
    g++;
  }
  else if(GtoB){
    if(g <= 0x01 && b >= 0xfe){
      GtoB = FALSE;
      BtoR = TRUE;
    }
    g--;
    b++;
  }
  else if(BtoR){
    if(b <= 0x01 && r >= 0xfe){
      BtoR = FALSE;
      RtoG = TRUE;
    }
    if(b == 0x01 && r == 0xfe){
      cycleFinished = TRUE;
    }
    b--;
    r++;
  }
  gfx->fillScreen(gfx->color565(r /* R */, g /* G */, b /* B */));
}

void printInfo() {
  Serial.print("RtoG: ");
  Serial.print(RtoG);
  Serial.print("\t");
  Serial.print("GtoB: ");
  Serial.print(GtoB);
  Serial.print("\t");
  Serial.print("BtoR: ");
  Serial.print(BtoR);
  Serial.print("\t");
  Serial.print("\t");
  Serial.print("R: 0x");
  if (r < 16) {
    Serial.print("0");
  }
  Serial.print(r, HEX);
  Serial.print("\t");
  Serial.print("G: 0x");
  if (g < 16) {
    Serial.print("0");
  }
  Serial.print(g, HEX);
  Serial.print("\t");
  Serial.print("B: 0x");
  if (b < 16) {
    Serial.print("0");
  }
  Serial.print(b, HEX);
  Serial.print("\t");
  Serial.print("\t");
  Serial.print("BTN: ");
  Serial.print(digitalRead(BTN) ? F("HIGH") : F("LOW"));
  Serial.println();
}

void button() {
  buttonState = digitalRead(BTN);
  if(buttonState == 0){
    buttonPressed = TRUE;
  }

  if(buttonPressed == TRUE){
    cycleFinished = FALSE;
    while(cycleFinished == FALSE){
      cycleRGB();
    }
    buttonPressed = FALSE;
    gfx->fillScreen(BLACK);
  }
}