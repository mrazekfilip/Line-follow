#define LEDs 7

class LineSensor{
public:
  int leds[5];
  LineSensor();
  void turnOn();
  void turnOff();
  void readLeds();
  void printLedsValue();
};


LineSensor::LineSensor(){
  pinMode(LEDs, OUTPUT);
  for (int i = 0; i < 5; i++){
    this->leds[i] = 0;
  }
}

void LineSensor::turnOn(){
  Serial.println("LEDs: On");
  digitalWrite(LEDs, HIGH);
}

void LineSensor::turnOff(){
  Serial.println("LEDs: Off");
  digitalWrite(LEDs, LOW);
}

void LineSensor::readLeds(){
  for (int i = 0; i < 5; i++){
    this->leds[i] = analogRead(i);
  }
}

void LineSensor::printLedsValue(){
  for (int i = 0; i < 5; i++){
    Serial.print(this->leds[i]);
    Serial.print(" ");
  }
  Serial.print("\n");
}
