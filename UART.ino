void UART_sendData(){
  byte sumData;
  sendData[0] = HEADER;
  sendData[1] = (byte)(pHvalue * 100.0) >> 8;
  sendData[2] = pHvalue * 100.0;
  sendData[3] = (byte)(tdsValue * 100.0) >> 8;
  sendData[4] = tdsValue * 100.0;
  sendData[5] = (byte)(ntuTurbidity * 100.0) >> 8;
  sendData[6] = ntuTurbidity * 100.0;
  for(int z = 1; z < 7; z++){
    sumData += sendData[z];
  }
  sendData[7] = 0 - sumData;
  mySerial.write(sendData, 7);
}
