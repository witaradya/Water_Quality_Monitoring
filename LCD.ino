/*
 * Menampilkan ke LCD pada saat alat pertama nyala
 */
void LCD_openingScreen(){ 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("MONITORING");
  lcd.setCursor(2, 1);
  lcd.print("KUALITAS AIR");
  delay(4000);
  lcd.setCursor(0, 0);
  lcd.print("ALDI BUDIAWAN");
  lcd.setCursor(2, 1);
  lcd.print("201952043");
  delay(4000);  
}


/*
 * Menampikan nilai sensor setiap saat
 */
void LCD_loop(){
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("pH ");
  lcd.setCursor(7, 0);
  lcd.print(pHvalue);
  
  lcd.setCursor(0, 1);
  lcd.print(tdsValue, 0);
  lcd.setCursor(4, 1);
  lcd.print("ppm");

  lcd.setCursor(8, 1);
  lcd.print(ntuTurbidity, 0);
  lcd.setCursor(13, 1);
  lcd.print("NTU");
}
