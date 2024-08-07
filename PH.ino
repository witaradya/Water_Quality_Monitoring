/*
 * Membaca data sensor pH
 */
void PH_read() {
  // Ambil data pH sebanyak 10 kali
  for (int w = 0; w < 10; w++)
  {
    buffer_arr[w] = analogRead(PIN_PH);
    delay(30);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  // Menghitung nilai rata - rata sensor pH pada 10 data yang sudah dibaca
  avgval = 0;
  for (int k = 2; k < 8; k++) avgval += buffer_arr[k];
  float volt = (float)avgval * 5.0 / 1024.0 / 6.0;

  // menghitung nilai pH
  pHvalue = -5.70 * volt + calibration_value;
  if(pHvalue > 8.0) pHvalue += 0.3;
  
  Serial.print("pH Value : ");
  Serial.println(pHvalue);
}

void PH_calibration() {
  // Membaca nilai pH dalam bentuk tegangan 0-5 V
  float Voltage = (float)analogRead(PIN_PH) * (5.0 / 1023.0);
  Serial.print("pH Volt : ");
  Serial.println(Voltage);
  delay(500);
}
