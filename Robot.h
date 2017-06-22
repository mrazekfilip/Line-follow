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
    motorL(6, 11, 9, 1.15), motorR(5, 10, 9, 1.15), lineSensor(), sonicSensor(), linePid(&lineSensor.error, &turning, &goodError, 0, 0, 0, DIRECT){
    this->goodError = 128;
    this->timeLine = millis();
    this->timeSonic = millis();
    this->timeMotors = millis();

    this->P = 0;
    this->I = 0;
    this->D = 0;
    this->i = 0;
    linePid.SetMode(AUTOMATIC);
}

void Robot::readSensors(){
    if (millis() - this->timeLine > 3){
        this->timeLine = millis();
        lineSensor.readLeds();
    }
    if (millis() - this->timeSonic > 100){
        this->timeSonic = millis();
        sonicSensor.readDistance();
    }

    if (millis() - this->timeMotors > 3){
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
            this->D += 0.1;
            break;
            case 'd':
            this->D -= 0.1;
            break;
        }
        linePid.SetTunings(P, I, D);
    }

    if (i == 50){
        Serial.print("error: ");
        Serial.print(lineSensor.error);
        Serial.print(" turning: ");
        Serial.print(this->turning);
        Serial.print(" motors:");
        Serial.print(motorL.absoluteSpeed);
        Serial.print(" ");
        Serial.print(motorR.absoluteSpeed);
        Serial.print(" PID: ");
        Serial.print(this->P);
        Serial.print(" ");
        Serial.print(this->I);
        Serial.print(" ");
        Serial.print(this->D);
        Serial.print(" ");
        Serial.print("\n");
        i = 0;
    }
    i++;

    if(this->turning >= 128){
        motorL.setSpeed(40);
        motorR.setSpeed(40 - (this->turning - 128) / 2.56);
    } else{
        motorL.setSpeed(40 + (this->turning - 128) / 2.56);
        motorR.setSpeed(40);

    }
}
