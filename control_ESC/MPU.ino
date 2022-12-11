void MPUsetup(){
  // Try to initialize!
  if (!mpu.begin()) {
    SerialBT.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  SerialBT.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  SerialBT.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    SerialBT.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    SerialBT.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    SerialBT.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    SerialBT.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  SerialBT.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    SerialBT.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    SerialBT.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    SerialBT.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    SerialBT.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  SerialBT.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    SerialBT.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    SerialBT.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    SerialBT.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    SerialBT.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    SerialBT.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    SerialBT.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    SerialBT.println("5 Hz");
    break;
  }
}
void MPUloop(){
  // Get new sensor events with the readings 
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Print out the values
  SerialBT.print("Acceleration X: ");
  SerialBT.print(a.acceleration.x);
  SerialBT.print(", Y: ");
  SerialBT.print(a.acceleration.y);
  SerialBT.print(", Z: ");
  SerialBT.print(a.acceleration.z);
  SerialBT.println(" m/s^2");

  SerialBT.print("Rotation X: ");
  SerialBT.print(g.gyro.x);
  SerialBT.print(", Y: ");
  SerialBT.print(g.gyro.y);
  SerialBT.print(", Z: ");
  SerialBT.print(g.gyro.z);
  SerialBT.println(" rad/s");

  SerialBT.print("Temperature: ");
  SerialBT.print(temp.temperature);
  SerialBT.println(" degC");

  SerialBT.println("");
  
    display.clearDisplay();
    display.setCursor(0,0);
    display.print(a.acceleration.x);
    display.setCursor(0,16);
    display.println(a.acceleration.y);
    display.setCursor(0,32);
    display.println(a.acceleration.z);
    display.setCursor(0,32);
    display.println(temp.temperature);
    display.display();
  delay(5000);
}
