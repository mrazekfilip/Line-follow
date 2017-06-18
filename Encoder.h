#include <AS5040.h>

#define EncoderDO 4
#define EncoderCLK 12
#define EncoderCS_R 8
#define EncoderCS_L 9

class Encoder{
public:
  AS5040 enc;
  int actual;
  unsigned long distance;
  float distanceCm;
  int delta;
  double speed;
  unsigned long actualMeasurement;
  float radius;
  Encoder(int EncoderCS, float radius);
  void readDistance();
  void setRadius(float radius);
  void printDistance();
};


Encoder::Encoder(int EncoderCS, float radius): enc(EncoderCLK, EncoderCS, EncoderDO), radius(radius){
  this->distance = 0;
  this->delta = 0;
  this->speed = 0;

  enc.begin();

  this->actualMeasurement = micros();
  this->actual = this->enc.read();
}

void Encoder::readDistance(){
  int last = this->actual;
  unsigned long lastMeasurement = this->actualMeasurement;

  this->actualMeasurement = micros();
  this->actual = this->enc.read();

  this->delta = this->actual - last;
  if (this->delta < 0){
    this->delta = 1024 + this->delta;
  }
  if (this->delta > 512){
    this->delta = -1024 + delta;
  }
  this->distance += delta;

  double deltaTime= (double) (this->actualMeasurement - lastMeasurement) / 1000000;
  speed = delta / deltaTime /1024 * 2 * this->radius * 3.14152;

  this->distanceCm = (double) this->distance / 1024 * 2 * this->radius * 3.14152;
}

void Encoder::setRadius(float radius){
  this->radius = radius;
}

void Encoder::printDistance(){
  Serial.print("Radius: ");
  Serial.print(this->radius);
  Serial.print(" Distance: ");
  Serial.print(this->distance);
  Serial.print(" Speed: ");
  Serial.print(this->speed);
  Serial.print("\n");
}
