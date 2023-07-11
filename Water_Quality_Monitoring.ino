#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include "GravityTDS.h"

#define PIN_TURBIDITY   A0
#define PIN_TDS         A1
#define PIN_PH          A2
#define PIN_RX          2
#define PIN_TX          3
#define HEADER          101

SoftwareSerial mySerial (PIN_RX, PIN_TX);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
GravityTDS gravityTds;

byte sendData[7];
byte checkSum;

float voltTurbidity = 0.0, ntuTurbidity = 0.0;
float temperature = 25.5, tdsValue = 0.0;

float calibration_value = 21.34 - 0.7;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
float pHvalue = 0.0;

unsigned long int myTime;

void INIT_Peripherals() {
  Serial.begin(115200);
  mySerial.begin(9600);
  
  pinMode(PIN_TURBIDITY, INPUT);
  pinMode(PIN_PH, INPUT);
  
  TDS_init();

  LCD_openingScreen();
}

void setup() {
  INIT_Peripherals();
}

void loop() {
  if((millis() - myTime) > 1000){
    PH_read();
    TDS_read();
    TURBIDITY_read();
    // PH_calibration();
    LCD_loop();
    UART_sendData();
    
    myTime = millis();
  }
}
