#include<PID_v1.h>
#include"Encoder.h"

class Motor{
private:
    int pinMotorF, pinMotorB;
    double absoluteSpeed;
    double speed;
    Encoder enc;
    PID pid;
    void sendSpeed();
public:
    Motor(int pinForward, int pinBackward, int pinEncoderCS, float radius);
    void setSpeed(double absoluteSpeed);
    double getAbsoluteSpeed();
    double getSpeed();
    void stop();
};

Motor::Motor(int pinForward, int pinBackward, int pinEncoderCS, float radius) :
        pinMotorF(pinForward), pinMotorB(pinBackward), enc(pinEncoderCS, radius), pid(&enc.speed, &speed, &absoluteSpeed, 2, 5, 1, DIRECT) {
    this->absoluteSpeed = 0;
    this->speed = 0;
    pid.SetMode(AUTOMATIC);
}

void Motor::sendSpeed(){
    if (this->speed > 255){
        this->speed = 255;
    }
    if (this->speed < -255){
        this->speed = -255;
    }

    if (this->speed >= 0){
        analogWrite(pinMotorF, (int) this->speed);
        analogWrite(pinMotorB, 0);
    } else{
        analogWrite(pinMotorB, (int) -this->speed);
        analogWrite(pinMotorF, 0);
    }
}

void Motor::stop(){
    this->speed = 0;
    sendSpeed();
}

double Motor::getAbsoluteSpeed(){
    return this->absoluteSpeed;
}

double Motor::getSpeed(){
    return this->speed;
}

void Motor::setSpeed(double absoluteSpeed){
    this->absoluteSpeed = absoluteSpeed;
    enc.readDistance();
    enc.printDistance();
    pid.Compute();
    sendSpeed();
}
