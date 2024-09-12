#include <SPI.h>

#define SS_PIN 5

volatile byte receivedData = 0; // Variable to store the received data
volatile bool dataReceived = false; // Flag to indicate data reception

void IRAM_ATTR onDataReceive() {
  if (digitalRead(SS_PIN) == LOW) {
    receivedData = SPI.transfer(0); // Read the received data
    dataReceived = true; // Set the flag to indicate data reception
  }
}

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  delay(1000);

  // Configure SPI settings
  SPI.begin(18, 19, 23, SS_PIN);
  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  // Configure SS pin
  pinMode(SS_PIN, INPUT_PULLUP);

  // Attach interrupt on SS pin
  attachInterrupt(digitalPinToInterrupt(SS_PIN), onDataReceive, FALLING);
}

void loop() {
  // Check if data has been received
  if (dataReceived) {
    // Process the received data
    Serial.print("Received Data: ");
    Serial.println(receivedData);

    // Reset the flag
    dataReceived = false;
  }
}
