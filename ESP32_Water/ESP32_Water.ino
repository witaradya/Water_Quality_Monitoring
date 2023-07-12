#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6axA7ifWp"
#define BLYNK_TEMPLATE_NAME "Water Quality Monitoring"
#define BLYNK_AUTH_TOKEN "2bjg6mWIOJSKaIpUG-SBq6hzDOWVFbzd"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define SSID     "monitorAir"
#define PASSWORD "12341234"

BlynkTimer timer;

float phValue = 0.0, tdsValue = 0.0, turbidityValue = 0.0;
float prevPH = 0.0, prevTDS = 0.0, prevTUR = 0.0;
byte dataInput[8];
byte checkSum;

unsigned long int myTimer = 0;
unsigned int countPH = 0, countTDS = 0, countTUR = 0;

void myTimerEvent()
{
  Blynk.virtualWrite(V2, turbidityValue);
  Blynk.virtualWrite(V1, tdsValue);
  Blynk.virtualWrite(V0, phValue);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, SSID, PASSWORD);
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();

  if (Serial2.available() > 6) {
    for (int a = 0; a < 7; a++) {
      dataInput[a] = Serial2.read();
      // Serial.print(dataInput[a]);
      // Serial.print("\t");
    }
    // Serial.println();
    
    if (dataInput[0] == 101){
      phValue = ((dataInput[1] << 8) + dataInput[2]) / 100.0;
      tdsValue = ((dataInput[3] << 8) + dataInput[4]);
      turbidityValue = ((dataInput[5] << 8) + dataInput[6]);
    }
    // Serial.print(phValue);
    // Serial.print("\t");
    // Serial.print(tdsValue);
    // Serial.print("\t");
    // Serial.println(turbidityValue);
  }

  if((millis() - myTimer) > 2000){
    if((prevPH > 9.0) || (prevPH < 4.0)) countPH++;
    if(prevTDS > 500.0) countTDS++;
    if(prevTUR > 2300.0) countTUR++;

    if((phValue > 9.0) || (phValue < 4.0)) countPH++;
    else countPH = 0;
    if(tdsValue > 500.0) countTDS++;
    else countTDS = 0;
    if(turbidityValue > 2300.0) countTUR++;
    else countTUR = 0;

    prevPH = phValue;
    prevTDS = tdsValue;
    prevTUR = turbidityValue;

    if(countPH >= 2){
      // Send notif to telegram
      countPH = 0;
    }
    if(countTDS >= 2){
      // Send notif to telegram
      countTDS = 0;
    }
    if(countTUR >= 2){
      // Send notif to telegram
      countTUR = 0;
    }
    myTimer = millis();
  }

}
