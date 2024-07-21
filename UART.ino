/*
 * Mengirim data semua sensor ke esp32
 */

void UART_sendData(){
  byte sumData;
  sendData[0] = 0xAA;
  sendData[1] = 0x10;
  sendData[2] = (uint16_t)(pHvalue * 100.0) >> 8;
  sendData[3] = (uint16_t)(pHvalue * 100.0);
  sendData[4] = (uint16_t)(tdsValue) >> 8;
  sendData[5] = (uint16_t)(tdsValue);
  sendData[6] = (uint16_t)(ntuTurbidity) >> 8;
  sendData[7] = (uint16_t)(ntuTurbidity);

  for(int a = 0; a < 8; a++){
    Serial.print(sendData[a]);
    Serial.print("\t");
  }
  Serial.println();
  mySerial.write(sendData, 8);
}
