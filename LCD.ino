void LCD_openingScreen(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("MONITORING");
  lcd.setCursor(1, 2);
  lcd.print("KUALITAS AIR");
  delay(5000);
}

void LCD_loop(){
  lcd.clear();
  lcd.setCursor(0, 3);
  lcd.print("pH ");
  lcd.setCursor(0, 6);
  lcd.print(pHvalue);
  
  lcd.setCursor(1, 1);
  lcd.print((int)tdsValue);
  lcd.setCursor(1, 4);
  lcd.print("ppm");

  lcd.setCursor(1, 8);
  lcd.print((int)ntuTurbidity);
  lcd.setCursor(1, 11);
  lcd.print("NTU");
}
