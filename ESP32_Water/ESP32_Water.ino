#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6axA7ifWp"
#define BLYNK_TEMPLATE_NAME "Water Quality Monitoring"
#define BLYNK_AUTH_TOKEN "2bjg6mWIOJSKaIpUG-SBq6hzDOWVFbzd"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "CTBot.h"

#define SSID     "monitorAir"
#define PASSWORD "12341234"

#define TOKEN_BOT "6305310374:AAEgphiqW_Wl6swhb6wgZMN8DznSi3vbYJA"
const int idChat = 1845118279;

BlynkTimer timer;
CTBot myBot;

float phValue = 0.0, tdsValue = 0.0, turbidityValue = 0.0;
float prevPH = 0.0, prevTDS = 0.0, prevTUR = 0.0;
byte dataInput[8];
byte checkSum;
byte countData;

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

  myBot.setTelegramToken(TOKEN_BOT);
  if (myBot.testConnection())
    Serial.println("\nBOT : TestConnection OK");
  else
    Serial.println("\nBOT : TestConnection NOK");
}

void loop() {
  Blynk.run();
  timer.run();
  TBMessage msg;

  if (Serial2.available() > 0) {
    byte data = Serial2.read();
    Serial.print(countData);
    Serial.print(" ");
    Serial.println(data);
    if (countData == 0) {
      if (data == 0xAA) {
        dataInput[0] = data;
        countData++;
      }
      else countData = 0;
    }
    else if (countData == 1) {
      if (data == 0x10) {
        dataInput[1] = data;
        countData++;
        return;
      }
      else countData = 0;
    }

    if (dataInput[0] == 0xAA && dataInput[1] == 0x10 && countData > 1) {
      dataInput[countData] = data;
      countData++;

      if (countData >= 8) {
        countData = 0;
        phValue = ((dataInput[2] << 8) + dataInput[3]) / 100.0;
        tdsValue = ((dataInput[4] << 8) + dataInput[5]);
        turbidityValue = ((dataInput[6] << 8) + dataInput[7]);
        Serial.print(phValue);
        Serial.print("\t");
        Serial.print(tdsValue);
        Serial.print("\t");
        Serial.println(turbidityValue);
      }
    }

    //    if (dataInput[0] == 101) {
    //      phValue = ((dataInput[1] << 8) + dataInput[2]) / 100.0;
    //      tdsValue = ((dataInput[3] << 8) + dataInput[4]);
    //      turbidityValue = ((dataInput[5] << 8) + dataInput[6]);
    //    }
    // Serial.print(phValue);
    // Serial.print("\t");
    // Serial.print(tdsValue);
    // Serial.print("\t");
    // Serial.println(turbidityValue);
  }

  if ((millis() - myTimer) > 2000) {
    if ((prevPH > 9.0) || (prevPH < 4.0)) countPH++;
    if (prevTDS > 500.0) countTDS++;
    if (prevTUR > 2300.0) countTUR++;

    if ((phValue > 9.0) || (phValue < 4.0)) countPH++;
    else countPH = 0;
    if (tdsValue > 1500.0) countTDS++;
    else countTDS = 0;
    if (turbidityValue > 25.0) countTUR++;
    else countTUR = 0;

    prevPH = phValue;
    prevTDS = tdsValue;
    prevTUR = turbidityValue;

    if (countPH >= 2) {
      // Send notif to telegram
      String nilaiPH = "nilai PH : ";
      nilaiPH += int(phValue);
      nilaiPH += "\nNILAI PH DILUAR BATAS!!!\n";
      myBot.sendMessage(idChat, nilaiPH);
      Serial.print("Mengirim data sensor ke telegram");

      countPH = 0;
    }
    if (countTDS >= 2) {
      // Send notif to telegram
      String nilaiTds = "nilai tds : ";
      nilaiTds += int(tdsValue);
      nilaiTds += "\nNILAI TDS MELAWATI BATAS\n";
      myBot.sendMessage(idChat, nilaiTds);
      Serial.print("Mengirim data sensor ke telegram");

      countTDS = 0;
    }
    if (countTUR >= 2) {
      // Send notif to telegram
      String nilaiTurbidity = "nilai turbidity : ";
      nilaiTurbidity += int(turbidityValue);
      nilaiTurbidity += "\nNILAI TURBIDITY MELAWATI BATAS !!\n";
      myBot.sendMessage(idChat, nilaiTurbidity);
      Serial.print("Mengirim data sensor ke telegram");

      countTUR = 0;
    }
    myTimer = millis();
  }

}
