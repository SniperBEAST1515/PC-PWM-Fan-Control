#define aref_voltage 3.3

// name all of the pins
int waterPin = 14;
int ambientPin = 15;
int pumpPWM = 3;
int fan1PWM = 9;
int fan2PWM = 10;
int pumpRPM = 2;
int fan1RPM = 4;
int fan2RPM = 7;


void setup() {
  // set the mode for each pin
  pinMode(waterPin, INPUT);
  pinMode(ambientPin, INPUT);
  pinMode(pumpPWM, OUTPUT);
  pinMode(fan1PWM, OUTPUT);
  pinMode(fan2PWM, OUTPUT);
  pinMode(pumpRPM, INPUT);
  pinMode(fan1RPM, INPUT);
  pinMode(fan2RPM, INPUT);
  
  // set PWM carrier frequency above audible threshold
  TCCR1B = TCCR1B & B11111000 | B00000001; // timer 1 controls pins 9 and 10 (fans)
  TCCR2B = TCCR2B & B11111000 | B00000001; // timer 2 controls pins 3 and 11 (pump)
  
  analogReference(EXTERNAL);
  
  // uncomment this to open up the serial monitor.
  Serial.begin(9600);
}

// loop to get this show on the road
void loop(){
  delay(75);
  int delta = deltaT();
  int fanPower = controlFan(delta);
  int pumpPower = controlPump(delta);
  
  analogWrite(fan1PWM, fanPower);
  analogWrite(fan2PWM, fanPower);
  analogWrite(pumpPWM, pumpPower);
  
  // uncomment serialMon to get a live readout of the current
  // analog readout delta, fan and pump duty cycles
  serialMon(delta, fanPower, pumpPower);
}

// map delta temp to fan and pump PWM duty cycle
int controlFan(int delta){
  int power = constrain((map(delta, 10, 30, 25, 255)), 25, 255);
  return power;
}

int controlPump(int delta){
  int power = constrain((map(delta, 10, 25, 30, 127)), 30, 127);
  return power;
}

// read temps
int deltaT() {
  int water = analogRead(waterPin);
  int ambient = analogRead(ambientPin);
  int delta = water - ambient;
  return(abs(delta));
}

// serial monitor
void serialMon(int delta, int fanPower, int pumpPower){
  Serial.print(delta);
  Serial.println(" - delta");
  Serial.print(fanPower);
  Serial.println(" - fan DC");
  Serial.print(pumpPower);
  Serial.println(" - pump DC");
  Serial.println();
}
