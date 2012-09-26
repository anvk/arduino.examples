// Controlling a servo position using a potentiometer (variable resistor) 
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
 
int potpin1 = 0;        // analog pin used to connect the potentiometer first light sensor
int potpin2 = 2;        // analog pin used to connect the potentiometer second light sensor
int analogVal1;         // sensor output for the first light sensor
int analogVal2;         // sensor output for the second light sensor
int analogValDiff;      // difference in readings for both light sensers
int sendData;           // data which will be sent to the servo
int maxServo = 179;     // Max potential data which will be sent to the servo
int minServo = 0;       // Min potential data which will be sent to the servo

int minAnalogDiffValue = -330;   // The minimum possible registered difference
int maxAnalogDiffValue = 300;    // The maximum possible registered difference 

int maxv = 0;          // for debug/adjusting purposes
int minv = 1000;       // for debug/adjusting purposes 

void setup() { 
  myservo.attach(9);    // attaches the servo on pin 9 to the servo object 
  //Serial.begin(9600);  
}

void readSensors() {
  analogVal1 = analogRead(potpin1);
  analogVal2 = analogRead(potpin2);
  analogValDiff = analogVal1 - analogVal2;
  
  if (maxv < analogValDiff) {
    maxv = analogValDiff;
  }
  
  if (minv > analogValDiff) {
    minv = analogValDiff;
  }
  
  sendData = maxServo - map(analogValDiff, minAnalogDiffValue, maxAnalogDiffValue, minServo, maxServo);     // scale it to use it with the servo (value between 0 and 180)
  //sensorOutputDebug();
}

void sensorOutputDebug() {
  Serial.print("1-");
  Serial.print(analogVal1);
  Serial.print(" 2-");
  Serial.print(analogVal2);
  Serial.print(" R-");
  Serial.print(analogValDiff);
  Serial.print(" max-");
  Serial.print(maxv);
  Serial.print(" min-");
  Serial.print(minv);
  Serial.print('\n');
}
 
void loop() {
  readSensors();                // read data from the light sensors
  myservo.write(sendData);      // sets the servo position according to the scaled value 
  delay(15);                    // waits for the servo to get there 
} 
