/*
 * Reference
 * https://how2electronics.com/diy-turbidity-meter-using-turbidity-sensor-arduino/
 */
void TURBIDITY_read(){
  for(int a = 0; a < 800; a++){
    voltTurbidity += (float)((analogRead(PIN_TURBIDITY)/1023.0) * 5.0);
  }
  voltTurbidity = voltTurbidity / 800.0;

  if(voltTurbidity < 2.50) ntuTurbidity = 3000.00;
  else ntuTurbidity = (-1120.4 * square(voltTurbidity)) + (5742.3 * voltTurbidity) - (4353.8);

  ntuTurbidity /= 100.0;

  Serial.print("Turbidity Value : ");
  Serial.print(ntuTurbidity);
  Serial.println(" NTU");
}

void TURBIDITY_readVolt(){
  float voltage = (float)analogRead(A0) * (5.0 / 1024.0);
 
  Serial.println ("Turbidity Output (V):");
  Serial.println (voltage);
  delay(1000);
}
