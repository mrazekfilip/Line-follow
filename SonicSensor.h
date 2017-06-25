#pragma once

#define SonicECHO 2
#define SonicTRIG 3

class SonicSensor{
public:
  float distance;
  SonicSensor();
  void readDistance();
  void printDistance();
};


SonicSensor::SonicSensor(){
  pinMode(SonicECHO, INPUT);
  pinMode(SonicTRIG, OUTPUT);
  this->distance = 50;
}

void SonicSensor::readDistance(){
    digitalWrite(SonicTRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(SonicTRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(SonicTRIG, LOW);

    this->distance = pulseIn(SonicECHO, HIGH);
    distance *= 0.017315f;
}

void SonicSensor::printDistance(){
  Serial.print(this->distance);
  Serial.print(" cm");
  Serial.print("\n");
}
