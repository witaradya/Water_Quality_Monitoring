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

SoftwareSerial mySerial (PIN_RX, PIN_TX); // Setting Pin 2 dan 3 sebagai pin serial untuk mengirim data ke ESP32
LiquidCrystal_I2C lcd(0x3F, 16, 2); // Setting LCD
GravityTDS gravityTds; // Setting sensor TDS

byte sendData[8];
byte checkSum;

float voltTurbidity = 0.0, ntuTurbidity = 0.0;
float temperature = 25.5, tdsValue = 0.0;

float calibration_value = 22.2; // Ganti nilai tersebut jika nilai pH tidak sesuai, jika nilai pH kurang tinggi makan tambah nilai tersebut, jika terlalu tinggi kurangi nilai tersebut
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
  // Setiap 1 detik sekali, baca sensor dan kirim kan data ke ESP32
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
