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
    double P, I, D;
public:
    Motor(int pinForward, int pinBackward, int pinEncoderCS, float radius);
    void setSpeed(double absoluteSpeed);
    double getAbsoluteSpeed();
    double getSpeed();
    void stop();
};

Motor::Motor(int pinForward, int pinBackward, int pinEncoderCS, float radius) :
        pinMotorF(pinForward), pinMotorB(pinBackward), enc(pinEncoderCS, radius), pid(&enc.speed, &speed, &absoluteSpeed, 1.5, 14, 0.1, DIRECT) {
    this->absoluteSpeed = 0;
    this->speed = 0;
    this->P = 1.5;
    this->I = 14;
    this->D = 0.1;
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
    char keystroke = ' ';
    this->absoluteSpeed = absoluteSpeed;

    if (Serial.available()>0){
        keystroke = Serial.read();

        switch(keystroke){
            case 'w':
            this->P += 0.1;
            break;
            case 's':
            this->P -= 0.1;
            break;
            case 'e':
            this->I += 0.1;
            break;
            case 'd':
            this->I -= 0.1;
            break;
            case 'r':
            this->D += 0.1;
            break;
            case 'f':
            this->D -= 0.1;
            break;
        }
        pid.SetTunings(P, I, D);
    }

    static int i = 0;

    if (i == 100){
        i = 0;
        Serial.print(pid.GetKp());
        Serial.print(" ");
        Serial.print(pid.GetKi());
        Serial.print(" ");
        Serial.print(pid.GetKd());
        Serial.print(" Output:");
        Serial.print(getSpeed());
        Serial.print(" Speed:");
        Serial.print(enc.speed);
        Serial.print("\n");
    }
    i++;


    enc.readDistance();
    pid.Compute();
    sendSpeed();
}
