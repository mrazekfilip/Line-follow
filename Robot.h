#include<PID_v1.h>
#include"Motor.h"
#include"LineSensor.h"
#include"SonicSensor.h";


class Robot{
private:
    Motor motorL;
    Motor motorR;
    LineSensor lineSensor;
    SonicSensor sonicSensor;
    PID linePid;
    double turning;
    double goodError;
    unsigned long timeLine;
    unsigned long timeSonic;
    unsigned long timeMotors;
    double P, I, D;
    void readSensors();
    void followLine();
    void findLine();
    void barrier();
    int i;
public:
    Robot();
    void start();
};

Robot::Robot() :
    motorL(6, 11, 9, 1.15), motorR(5, 10, 8, 1.15), lineSensor(), sonicSensor(), linePid(&lineSensor.error, &turning, &goodError, 0.880, 0.075, 0.19, DIRECT){
    this->goodError = 0;
    this->timeLine = millis();
    this->timeSonic = millis();
    this->timeMotors = millis();

    this->P = 0.88;
    this->I = 0.08;
    this->D = 0.02;
    this->i = 0;
    linePid.SetMode(AUTOMATIC);
    linePid.SetOutputLimits(-300, 300);
    motorL.pid.SetOutputLimits(-255, 255);
    motorR.pid.SetOutputLimits(-255, 255);
}

void Robot::readSensors(){
    if (millis() - this->timeLine > 4){
        this->timeLine = millis();
        lineSensor.readLeds();
    }
    if (millis() - this->timeSonic > 100){
        this->timeSonic = millis();
        sonicSensor.readDistance();
    }

    if (millis() - this->timeMotors > 15){
        this->timeMotors = millis();
        motorL.enc.readDistance();
        motorR.enc.readDistance();
    }
}

void Robot::start(){
    readSensors();


    if (lineSensor.seeLine){
        followLine();
    } else{
        motorL.stop();
        motorR.stop();
    }

    // if (sonicSensor.distance < 20){
    //     Serial.println("not implemented: barrier");
    //     //barrier();
    // } else if (!lineSensor.seeLine){
    //     Serial.println("not implemented: findLine");
    //     //findLine();
    // }
    // else{
    //     followLine();
    // }
}

void Robot::followLine(){
    linePid.Compute();


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
            this->I += 0.1;
            break;
            case 's':
            this->I -= 0.1;
            break;
            case 'e':
            this->D += 0.01;
            break;
            case 'd':
            this->D -= 0.01;
            break;
            case 'r':
            this->P += 0.01;
            break;
            case 'f':
            this->P -= 0.01;
            break;
            case 't':
            this->I += 0.001;
            break;
            case 'g':
            this->I -= 0.001;
            break;
            case 'z':
            this->D += 0.001;
            break;
            case 'h':
            this->D -= 0.001;
            break;
        }
        linePid.SetTunings(P, I, D);
    }

    if (i == 50){
        Serial.print("error: ");
        Serial.print(lineSensor.error);
        Serial.print(" turning: ");
        Serial.print(" ");
        Serial.print(this->turning);
        Serial.print(" ");
        Serial.print(" motors:");
        Serial.print(motorL.absoluteSpeed);
        Serial.print(" ");
        Serial.print(motorR.absoluteSpeed);
        Serial.print(" PID: ");
        Serial.print(this->P, 3);
        Serial.print(" ");
        Serial.print(this->I, 3);
        Serial.print(" ");
        Serial.print(this->D, 3);
        Serial.print(" ");
        Serial.print("\n");
        i = 0;
    }
    i++;

    motorL.setSpeed(18 + this->turning / 13.63);
    motorR.setSpeed(-18 + this->turning / 13.63);
}

void Robot::barrier(){
    //rotate left
    delay(20);
    double startDistance = motorL.enc.distanceCm;
    while(abs(startDistance - motorL.enc.distanceCm) < 9){
        readSensors();
        Serial.print("left: ");
        Serial.println(startDistance - motorL.enc.distanceCm);
        motorL.setSpeed(-20);
        motorR.setSpeed(-20);
    }
    //forward 15 cm
    delay(20);
    startDistance = motorL.enc.distanceCm;
    while(abs(startDistance - motorL.enc.distanceCm) < 15){
        readSensors();
        Serial.print("forward: ");
        Serial.println(startDistance - motorL.enc.distanceCm);
        motorL.setSpeed(30);
        motorR.setSpeed(-30);
    }
    //rotate right
    delay(20);
    startDistance = motorL.enc.distanceCm;
    while(abs(motorL.enc.distanceCm - startDistance) < 9){
        readSensors();
        Serial.print("right: ");
        Serial.println(motorL.enc.distanceCm - startDistance);
        motorL.setSpeed(20);
        motorR.setSpeed(20);
    }
}

void Robot::findLine(){
    unsigned long startTime = millis();
    while(!lineSensor.seeLine){
        readSensors();
        Serial.println(sin(sqrt(millis() - startTime)/4));
        if (sin(sqrt(millis() - startTime)/4) >= 0){
            motorL.setSpeed(10);
            motorR.setSpeed(-30);
        } else{
            motorL.setSpeed(30);
            motorR.setSpeed(-10);
        }
    }
}
