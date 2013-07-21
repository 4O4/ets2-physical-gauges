/* 
  ETS 2 physical gauges controller 

  Example code for Arduino board. It's receiving data from serial port (sent by telemetry_client) 
  and parsing them. Then it's mapping result to gauge scale and setting servo position.

  Author: Pawelo, <pk.pawelo@gmail.com> (GitHub: https://github.com/4O4)
  License: MIT


  The MIT License (MIT)
  
  Copyright (c) 2013 Paweł K. (Pawelo)
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software. 

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <Servo.h>

// Specify your servo range (in microseconds)
#define SERVO_MIN_RANGE 400
#define SERVO_MAX_RANGE 2800

// Choose pin for each
#define SERVO_SPEEDO_PIN 2   
#define SERVO_RPM_PIN 4      

char     data1[10], data2[10];
boolean  shift        = false;
Servo    servo_rpm, servo_speedo;

void setup() {
  Serial.begin(115200);
  servo_speedo.attach(SERVO_SPEEDO_PIN, SERVO_MIN_RANGE, SERVO_MAX_RANGE);
  servo_rpm.attach(SERVO_RPM_PIN, SERVO_MIN_RANGE, SERVO_MAX_RANGE);
}

void loop() {
  // Nothing to do here...
}

void serialEvent() {
  while (Serial.available()) {

    // Code below needs major modifications if you want to receive more data than just RPMs and Speed

    // Reading data from serial port, char by char (structure of data: "RPMs|Speed#")
    char input = (char)Serial.read(); 
    
    // Basic parsing, switches the variable which will store next characters when separator ("|") appears
    if (input == '|') {
      shift = true;
      continue;
    }
    
    // Adding each character of section to proper temporary variable
    if (shift) {
      strncat(data2, &input, 1);  
    } else {
      strncat(data1, &input, 1);
    }
      
    // End of message character, main program
    if (input == '#') {

      // Converting C-string to int
      int truck_speed = atoi(data2); 
      int engine_rpm = atoi(data1);
      
      // Mapping values
      int speed_uS = map(truck_speed, 110, 0, SERVO_MIN_RANGE, SERVO_MAX_RANGE);
      int rpm_uS = map(engine_rpm, 2500, 0, SERVO_MIN_RANGE, SERVO_MAX_RANGE);

      servo_rpm.writeMicroseconds(rpm_uS);
      servo_speedo.writeMicroseconds(speed_uS);
    
      // Resetting temporary variables
      shift = false;
      memset(data1, 0, strlen(data1));
      memset(data2, 0, strlen(data2));
    } 
  }
}
