void PH_read() {
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
  avgval = 0;
  for (int k = 2; k < 8; k++) avgval += buffer_arr[k];
  float volt = (float)avgval * 5.0 / 1024.0 / 6.0;
  pHvalue = -5.70 * volt + calibration_value;

  Serial.print("pH Value : ");
  Serial.println(pHvalue);
}

void PH_calibration() {
  float Voltage = (float)analogRead(PIN_PH) * (5.0 / 1023.0);
  Serial.print("pH Volt : ");
  Serial.println(Voltage);
  delay(500);
}
