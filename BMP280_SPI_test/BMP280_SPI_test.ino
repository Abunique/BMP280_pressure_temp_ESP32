

#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCL 18//SCK (Serial Clock, output from master)..........SCL
#define BMP_SDO 19// MISO(Master IN Slave OUT).................................SDO
#define BMP_SDA 23 //MOSI (Master OUT Slave IN)................................SDA
#define BMP_CSB1 5// (Slave Select).......................................................CSB


Adafruit_BMP280 bmp1(BMP_CSB1, BMP_SDA, BMP_SDO, BMP_SCL);



void setup() {
  Serial.begin(9600);
  
  Serial.println("Starting BMP280 device 1...");


  if (!bmp1.begin()) {
    Serial.println("Sensor BMP280 device 1 was not found.");
    while (1);
  }
  Serial.println("Initialize BMP280 1 completed.");

  delay(2000);

  
}  

void loop() {
  float pressure = bmp1.readPressure() / 100.0;
  Serial.print("\nPressure = ");
  Serial.print(pressure);
  Serial.print("Pa,   ");

  delay(100);
 float temperature = bmp1.readTemperature();
  Serial.print("\nTemperature = ");
  Serial.print(temperature);
  Serial.print("cel,   ");
}