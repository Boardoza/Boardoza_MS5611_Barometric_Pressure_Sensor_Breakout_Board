#include "MS5611.h"

MS5611 sensor;

void setup() {
  Serial.begin(115200);

  // Initialize sensor
  if(sensor.begin(ULTRA_HIGH_RES)) {
    Serial.println("MS5611 initiated.");
  } else {
    Serial.println("MS5611 failed to start.");
    while(1); // Stay in loop if sensor fails to initialize
  }
}

void loop() {
  // Read and print the temperature value
  double temperature = sensor.readTemperature(true); // Read the temperature value (compensate)
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C  ");

  // Read and print the pressure value
  int32_t pressure = sensor.readPressure(true); // Read the pressure value (compensate)
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.print(" Pa ");

  // Calculate and print the pressure value at sea level
  double seaLevelPressure = 101325; // Example: 101325 Pa
  double altitude = sensor.getAltitude(pressure, seaLevelPressure); // Calculate height
  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  delay(1000); // Wait for 1 second
}