/*
 * Reference
 * https://how2electronics.com/tds-sensor-arduino-interfacing-water-quality-monitoring/
 * Library
 * https://github.com/DFRobot/GravityTDS.git
 */
void TDS_init(){
  gravityTds.setPin(PIN_TDS);
  gravityTds.setAref(5.0);
  gravityTds.setAdcRange(1024);
  gravityTds.begin();
}

void TDS_read(){
  gravityTds.setTemperature(temperature);
  gravityTds.update(); 
  tdsValue = gravityTds.getTdsValue();

  Serial.print("TDS Value : ");
  Serial.print(tdsValue);
  Serial.println(" ppm");
}
