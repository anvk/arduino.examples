int sensorPin = A2;      // select the input pin for the potentiometer
int ledPin = A0;         // select the pin for the LED
int sensorValue = 0;     // variable to store the value coming from the sensor
int outputValue = 0;     // value output to the PWM (analog out)
int maxDelay = 9;        // Delay between lights being on and off for a "dimm effect"
int minSensorValue = 10; // sensor in complete darkness but with my monitor on :)
int maxSensorValue = 40; // sensor value which is shown in my room with light on at night
int newOutputValue = -1; // new registered output value. We will require it for a slow transition between bright to dimm and vice versa
int count = 20;          // time for a light to be in a specific state. Total transition time = count * maxDelay in ms. In this exaple it is ~200ms for 1 state.
// Total transition time is count * maxDelay * maxDelay ~= 2 seconds

// setup our board
void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);  
  //Serial.begin(9600); // For debugging purposes
  // First initial read of inpiuts
  readInputs();
  // Save output value
  outputValue = newOutputValue;
}

void light(int environmentLuminosity) {
  // Make lights to appear dimmer depending on environmental luminosity
  int lightOn = maxDelay - environmentLuminosity;
  int lightOff = environmentLuminosity;
  digitalWrite(ledPin, HIGH);
  delay(lightOn);
  digitalWrite(ledPin, LOW);
  delay(lightOff);
}

void delayedLight(int environmentLuminosity, int howLong) {
  // Basically keep the light on for howLong * maxDelay time
  for (int i=0; i<howLong; i++) {
    light(environmentLuminosity);
    //debug(environmentLuminosity); // For debugging purposes
  }
}

void debug(int val) {
  Serial.print(val);
  Serial.print('\n');
}

void sensorAndOutputDebug() {
  Serial.print("S-");
  Serial.print(sensorValue);
  Serial.print(" O-");
  Serial.print(outputValue);
  Serial.print('\n');
}

void readInputs() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  // let's tweak the value for my room light at night
  sensorValue = (sensorValue > maxSensorValue) ? maxSensorValue : sensorValue;
  sensorValue = (sensorValue < minSensorValue) ? minSensorValue : sensorValue;
  // Map the value to set the proper 1-9 range for our light to blink
  newOutputValue = map(sensorValue, minSensorValue, maxSensorValue, 1, maxDelay-1);
}

void loop() {
  // read our inputs
  readInputs();

  // If the new registered value differs from the old one then slowly change it
  if (newOutputValue > outputValue) {
    outputValue++;
  } else if (newOutputValue < outputValue) {
    outputValue--;
  }
  
  // Adjust light accordingly and keep it on for the count * maxDelay time till the next loop run where most likely we will repeat the process since light has changed
  delayedLight(outputValue, count);
}


