#pragma once

#define LEDs 7

class LineSensor{
public:
  int leds[5];
  int weights[5];
  int calib[5];
  double error;
  bool seeLine;
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
  this->error = 0;
  this->weights[0] = -3;
  this->weights[1] = -2;
  this->weights[2] = 0;
  this->weights[3] = 2;
  this->weights[4] = 3;


  this->calib[0] = 0;
  this->calib[1] = -5;
  this->calib[2] = -20;
  this->calib[3] = -5;
  this->calib[4] = 26;

  this->error = 0;
  this->seeLine = true;

}

void LineSensor::turnOn(){
  digitalWrite(LEDs, HIGH);
}

void LineSensor::turnOff(){
  digitalWrite(LEDs, LOW);
}

void LineSensor::readLeds(){
  int tempLeds[5] = {0, 0, 0, 0, 0};
  turnOn();
  delay(1);
  for (int i = 0; i < 5; i++){
    tempLeds[i] = analogRead(i);
  }
  turnOff();
  delay(1);
  for (int i = 0; i < 5; i++){
    this->leds[i] = analogRead(i) - tempLeds[i] + this->calib[i];
  }

  this->seeLine = false;
  for (int i = 0; i < 5; i++){
      if (this->leds[i] < 100){
          this->seeLine = true;
      }
  }

  this->error = 0;
  for (int i = 0; i <5; i++){
      this->error += this->leds[i] * this->weights[i];
  };
}

void LineSensor::printLedsValue(){
  Serial.print("led values: ");
  for (int i = 0; i < 5; i++){
    Serial.print(this->leds[i]);
    Serial.print(" ");
  }
  Serial.print("error: ");
  Serial.print(this->error);
  Serial.print("\n");
}
