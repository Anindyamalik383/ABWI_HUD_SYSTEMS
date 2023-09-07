#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345); // 12345 is the sensor ID

const int trigger=9;

void setup() {
  Serial.begin(9600);

  pinMode(trigger,OUTPUT);


  if(!accel.begin()) {
    Serial.println("Failed to initialize ADXL345 accelerometer.");
    while(1);
  }
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);
  
  float acceleration_magnitude = sqrt(pow(event.acceleration.x, 2) + pow(event.acceleration.y, 2) + pow(event.acceleration.z, 2));
  Serial.println(acceleration_magnitude);
  float crash_threshold = 20.0; // adjust this value as necessary
  if (acceleration_magnitude > crash_threshold) {
    Serial.println("Crash detected!");
    digitalWrite(trigger,LOW);
    
  }
else{
  digitalWrite(trigger,HIGH);
  
}
  Serial.println(trigger);
  delay(100); // wait for a short period of time before reading the sensor data again
}
