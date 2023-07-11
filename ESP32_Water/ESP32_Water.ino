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
byte dataInput[8];
byte checkSum;

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
}
