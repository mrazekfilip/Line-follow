#pragma once
#include<PID_v1.h>
#include"Encoder.h"

class Motor{
public:
    int pinMotorF, pinMotorB;
    double absoluteSpeed;
    double speed;
    Encoder enc;
    PID pid;
    void sendSpeed();
    double P, I, D;
    Motor(int pinForward, int pinBackward, int pinEncoderCS, float radius);
    void setSpeed(double absoluteSpeed);
    double getAbsoluteSpeed();
    double getSpeed();
    void stop();
    void print();
};

Motor::Motor(int pinForward, int pinBackward, int pinEncoderCS, float radius) :
        pinMotorF(pinForward), pinMotorB(pinBackward), enc(pinEncoderCS, radius), pid(&enc.speed, &speed, &absoluteSpeed, 3, 0.02, 0.002, DIRECT) {
    this->absoluteSpeed = 0;
    this->speed = 0;
    this->P = 1.86;
    this->I = 0.005;
    this->D = 0.001;
    //3.36, 0.83, 0.02
    //pid.SetOutputLimits(-255, 255);
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

    char keystroke = ' ';
    if (Serial.available()>0){
        keystroke = Serial.read();

        switch(keystroke){
            case 'q':
            this->P += 0.1;
             break;
            case 'a':
            this->P -= 0.1;
            break;
            case 'w':
            this->I += 0.001;
            break;
            case 's':
            this->I -= 0.001;
            break;
            case 'e':
            this->D += 0.001;
            break;
            case 'd':
            this->D -= 0.001;
            break;
        }
        pid.SetTunings(P, I, D);
    }
    enc.readDistance();
    pid.Compute();
    sendSpeed();
}

void Motor::print(){
    Serial.print("Radius: ");
    Serial.print(this->enc.radius);
    Serial.print(" Distance: ");
    Serial.print(this->enc.distance);
    Serial.print(" Speed: ");
    Serial.print(this->enc.speed);
    Serial.print(" send speed: ");
    Serial.print(this->speed);
    Serial.print("\n");
}
