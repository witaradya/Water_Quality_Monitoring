#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
// Kode untuk terhubung ke akun Blynk
#define BLYNK_TEMPLATE_ID "TMPL6axA7ifWp"
#define BLYNK_TEMPLATE_NAME "Water Quality Monitoring"
#define BLYNK_AUTH_TOKEN "2bjg6mWIOJSKaIpUG-SBq6hzDOWVFbzd"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h> // Library untuk terhubung ke BLYNK
#include "CTBot.h"            // Library untuk BOT TELEGRAM
#include <HTTPClient.h>

#define SSID     "monitorAir" // Nama WiFi yang akan dihubungkan 
#define PASSWORD "12341234"   // Password WiFi yang akan dihubungkan

#define TOKEN_BOT "6305310374:AAEgphiqW_Wl6swhb6wgZMN8DznSi3vbYJA"  // Token untuk terhubung ke bot telegram
const int idChat = 1845118279; // ID Bot telegram

/*This is the Sheets GAS ID, you need to look for your sheets id*/                       
String Sheets_GAS_ID = "AKfycbxxb84il6P9jvmbRT73QWC9kN3ZtKh22KrJs6U6yhFv6hnwDbZLGhI9EyfDzSiss6GXmA";                                                

BlynkTimer timer;
CTBot myBot;

float phValue = 0.0, tdsValue = 0.0, turbidityValue = 0.0; // Variabel untuk menyimpan nilai sensor dari arduino nano
float prevPH = 0.0, prevTDS = 0.0, prevTUR = 0.0;          // variable untuk menyimpan nilai sensor sebelumnya
byte dataInput[8];
byte checkSum;
byte countData;

unsigned long int myTimer = 0;
unsigned int countPH = 0, countTDS = 0, countTUR = 0;


// Untuk mengirim data ke BLYNK
void myTimerEvent()
{
  Blynk.virtualWrite(V2, turbidityValue);
  Blynk.virtualWrite(V1, tdsValue);
  Blynk.virtualWrite(V0, phValue);
}

void WRITE_GoogleSheet(){
  String urlFinal = "https://script.google.com/macros/s/"+Sheets_GAS_ID+"/exec?"+"value1=" + String(turbidityValue) + "&value2=" + String(tdsValue) + "&value3=" + String(phValue);
  Serial.print("POST data to spreadsheet:");
  Serial.println(urlFinal);
  HTTPClient http;
  http.begin(urlFinal.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET(); 
  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode);

  String payload;
  if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Payload: "+payload);    
  }
  http.end();
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);

  // Mulai menghubungkan ke BLYNK
  Blynk.begin(BLYNK_AUTH_TOKEN, SSID, PASSWORD);
  // Membuat timer 1 detik sekali untuk mengirim data ke BLYNK
  timer.setInterval(1000L, myTimerEvent);
  
  // Mulai menghubungkan ke BOT
  myBot.setTelegramToken(TOKEN_BOT);
  if (myBot.testConnection())
    Serial.println("\nBOT : TestConnection OK");
  else
    Serial.println("\nBOT : TestConnection NOK");        
}

void loop() {
  Blynk.run();      // Menjalan kan fungsi BLYNK
  timer.run();      // Menjalankan Timer untuk mengirim data ke BLYNK setiap 1 detik sekali
  TBMessage msg;

  //  Menunggu data dari arduino nano
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
  }

  // Menjalankan Timer setiap 2 detik sekali untuk memeriksa apakah ada nilai yang melebihi batas
  // Jika nilai sensor melebihi batas, maka akan mengirim chat ke Telegram 
  if ((millis() - myTimer) > 2000) {
    // Menyimpan data sensor sebelumnya
    if ((prevPH > 9.0) || (prevPH < 4.0)) countPH++;
    if (prevTDS > 300.0) countTDS++;
    if (prevTUR > 25.0) countTUR++;

    // Memeriksa batas nilai sensor  apakah melebihi batas
    if ((phValue > 9.0) || (phValue < 4.0)) countPH++; 
    else countPH = 0;
    if (tdsValue > 300.0) countTDS++;
    else countTDS = 0;
    if (turbidityValue > 25.0) countTUR++;
    else countTUR = 0;

    prevPH = phValue;
    prevTDS = tdsValue;
    prevTUR = turbidityValue;

    // Mengirim Notifikasi ke telegram jika nilai PH diluar batas
    if (countPH >= 4) {
      // Send notif to telegram
      String nilaiPH = "nilai PH : ";
      nilaiPH += int(phValue);
      nilaiPH += "\nNILAI PH DILUAR BATAS!!!\n"; // Ganti kalimat yang ada dalam tanda "" untuk mengganti pesan notifikasi ke telegram
      myBot.sendMessage(idChat, nilaiPH);
      Serial.print("Mengirim data sensor ke telegram");

      countPH = 0;
    }
    // Mengirim notifikasi ke telegram jika nilai TDS melebihi batas
    if (countTDS >= 4) {
      // Send notif to telegram
      String nilaiTds = "nilai tds : ";
      nilaiTds += int(tdsValue);
      nilaiTds += "\nNILAI TDS MELAWATI BATAS\n"; // Ganti kalimat yang ada dalam tanda "" untuk mengganti pesan notifikasi ke telegram
      myBot.sendMessage(idChat, nilaiTds);
      Serial.print("Mengirim data sensor ke telegram");

      countTDS = 0;
    }
    // Mengirim notifikasi ke telegram jika nilai Turbidity melebihi batas
    if (countTUR >= 4) {
      // Send notif to telegram
      String nilaiTurbidity = "nilai turbidity : ";
      nilaiTurbidity += int(turbidityValue);
      nilaiTurbidity += "\nNILAI TURBIDITY MELAWATI BATAS !!\n"; // Ganti kalimat yang ada dalam tanda "" untuk mengganti pesan notifikasi ke telegram
      myBot.sendMessage(idChat, nilaiTurbidity);
      Serial.print("Mengirim data sensor ke telegram");

      countTUR = 0;
    }

    // send data to excel
    WRITE_GoogleSheet();
    myTimer = millis();
  }
}
