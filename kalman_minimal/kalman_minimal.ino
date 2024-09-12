/* 
 * Description : Collesting Temperature data from BMP280 sensor and apply Kalman filter for noise reduction,
 * Sleep wake up routine implementation to wake up every 10 seconds acquire data for 5 seconds ,compute values.
 * 
 * Author:
 * Abinaya S
 * 
 * Revision:NA
 *
 */

#include <Kalman.h>
#include <Adafruit_BMP280.h>

using namespace BLA;

//------------------------------------
/****       BMP280 PARAMETERS    ****/
//------------------------------------

//SPI communication for acquiring sensor data
#define BMP_SCL 18//SCK (Serial Clock, output from master)..........SCL
#define BMP_SDO 19// MISO(Master IN Slave OUT).................................SDO
#define BMP_SDA 23 //MOSI (Master OUT Slave IN)................................SDA
#define BMP_CSB1 5// (Slave Select).......................................................CSB
// creating a object for the class 
Adafruit_BMP280 bmp1(BMP_CSB1, BMP_SDA, BMP_SDO, BMP_SCL);


//------------------------------------
/****       KALMAN PARAMETERS    ****/
//------------------------------------
// Dimensions of the matrices
#define Nstate 2 // length of the state vector
#define Nobs 2   // length of the measurement vector

// measurement std (to be characterized from your sensors)
#define n1 0.2 // noise on the 1st measurement component
#define n2 0.1 // noise on the 2nd measurement component 

// model std (~1/inertia). Freedom you give to relieve your evolution equation
#define m1 0.01
#define m2 0.02

KALMAN<Nstate,Nobs> K; // your Kalman filter
BLA::Matrix<Nobs> obs; // observation vector

// Note: I made 'obs' a global variable so memory is allocated before the loop.
//       This might provide slightly better speed efficiency in loop.


//-----------------------------------
/****           SETUP           ****/
//-----------------------------------

void setup() {

  Serial.begin(9600);

//senosr initialization
  if (!bmp1.begin()) {
    Serial.println("Sensor BMP280 device was not found.");
    while (1);
  }
  Serial.println("Initialize BMP280 completed.");
  delay(2000);
  
  //KAlMAN parameters initialization
  // example of evolution matrix. Size is <Nstate,Nstate>
  K.F = {1.0, 0.0,
         0.0, 1.0};
  // example of measurement matrix. Size is <Nobs,Nstate>
  K.H = {1.0, 0.0,
         0.0, 1.0};
  // example of measurement covariance matrix. Size is <Nobs,Nobs>
  K.R = {n1*n1,   0.0,
           0.0, n2*n2};
  // example of model covariance matrix. Size is <Nstate,Nstate>
  K.Q = {m1*m1,   0.0,
           0.0, m2*m2};
  
}

//-----------------------------------
/****            LOOP           ****/
//-----------------------------------

void loop() {

  // eventually update your evolution matrix inside the loop
  K.F = {1.0,  0.2,
         0.0,  1.0};
  
  // GRAB MEASUREMENT and WRITE IT INTO 'obs'
  get_sensor_data();
  
  // APPLY KALMAN FILTER
  K.update(obs);

  // PRINT RESULTS: measures and estimated state
  Serial << obs << ' ' << K.x << '\n';
}

//-----------------------------------
/****     GET SENSOR DATA       ****/
//-----------------------------------

//sensor data collection time interval
const int sampleInterval = 500;  // Interval between samples in milliseconds (500 milliseconds)
const int totalDuration = 5000;   // Total duration for data collection (5 seconds in milliseconds)
const int numSamples = totalDuration / sampleInterval; // Total number of samples//10 samples in 5 seconds

void get_sensor_data(){

for (int i = 0; i < numSamples; i++) {
  float temp = bmp1.readTemperature();
  obs(i) = temp;
  // Print the data to the Serial Monitor
  Serial.print("Temperature sample:");
  Serial.print(i + 1);
  Serial.print(": temperature Value");
  Serial.println(temp);
  Serial.print(" 'C");
}
}
