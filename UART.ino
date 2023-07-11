void UART_sendData(){
  byte sumData;
  sendData[0] = HEADER;
  sendData[1] = (uint16_t)(pHvalue * 100.0) >> 8;
  sendData[2] = (uint16_t)(pHvalue * 100.0);
  sendData[3] = (uint16_t)(tdsValue) >> 8;
  sendData[4] = (uint16_t)(tdsValue);
  sendData[5] = (uint16_t)(ntuTurbidity) >> 8;
  sendData[6] = (uint16_t)(ntuTurbidity);

  for(int a = 0; a < 7; a++){
    Serial.print(sendData[a]);
    Serial.print("\t");
  }
  Serial.println();
  mySerial.write(sendData, 7);
}
