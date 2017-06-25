#pragma once

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
public:
    Robot();
    void start();
};

Robot::Robot() :
    motorL(6, 11, 9, 1.15), motorR(5, 10, 8, 1.15), lineSensor(), sonicSensor(), linePid(&lineSensor.error, &turning, &goodError, 1, 0.07, 0.018, DIRECT) {
    //0.880, 0.075, 0.019
    this->goodError = 0;
    this->timeLine = millis();
    this->timeSonic = millis();
    this->timeMotors = millis();

    this->P = 1;
    this->I = 0.6;
    this->D = 0.19;
    linePid.SetMode(AUTOMATIC);
    linePid.SetOutputLimits(-400, 400);
    motorL.pid.SetOutputLimits(-255, 255);
    motorR.pid.SetOutputLimits(-255, 255);
}

void Robot::readSensors(){
    if (millis() - this->timeLine > 4){
        this->timeLine = millis();
        lineSensor.readLeds();
    }
    if (millis() - this->timeSonic > 500){
        this->timeSonic = millis();
        sonicSensor.readDistance();
    }

    if (millis() - this->timeMotors > 10){
        this->timeMotors = millis();
        motorL.enc.readDistance();
        motorR.enc.readDistance();
    }
}

void Robot::start(){
    readSensors();

    // if (lineSensor.seeLine){
    //     followLine();
    // } else {
    //     findLine();
    // }

    if (sonicSensor.distance < 15){
        barrier();
    } else if (lineSensor.seeLine){
        followLine();
    }
    else{
        findLine();
    }
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

    static int i = 0;
    if (i == 30){
        Serial.print("error: ");
        Serial.print(lineSensor.error);
        Serial.print(" turning: ");
        Serial.print(" ");
        Serial.print(this->turning);
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

    motorL.setSpeed(18 + this->turning / 18.19);
    motorR.setSpeed(-18 + this->turning / 18.19);
}

void Robot::barrier(){
    motorL.stop();
    motorR.stop();
    delay(200);

    unsigned long startTime = millis();

    //right
    startTime = millis();
    while(millis() - startTime < 500){
        readSensors();
        motorL.setSpeed(30);
        motorR.setSpeed(30);
    }

    motorL.stop();
    motorR.stop();
    delay(200);

    //forward
    startTime = millis();
    while(millis() - startTime < 800){
        readSensors();
        motorL.setSpeed(33);
        motorR.setSpeed(-30);
    }
    motorL.stop();
    motorR.stop();
    delay(200);

    //left
    startTime = millis();
    while(millis() - startTime < 350){
        readSensors();
        motorL.setSpeed(-30);
        motorR.setSpeed(-30);
    }

    motorL.stop();
    motorR.stop();
    delay(200);

    //forward
    startTime = millis();
    while(millis() - startTime < 1500){
        readSensors();
        motorL.setSpeed(33);
        motorR.setSpeed(-30);
    }
    motorL.stop();
    motorR.stop();
    delay(200);

    //left
    startTime = millis();
    while(millis() - startTime < 350){
        readSensors();
        motorL.setSpeed(-30);
        motorR.setSpeed(-30);
    }

    motorL.stop();
    motorR.stop();
    delay(200);

    findLine();
}

void Robot::findLine(){
    motorL.stop();
    motorR.stop();

    delay(200);

    unsigned long startTime = millis();

    //right
    startTime = millis();
    while(millis() - startTime < 220){
        readSensors();
        motorL.setSpeed(30);
        motorR.setSpeed(30);
    }

    motorL.stop();
    motorR.stop();

    delay(200);

    //forward
    startTime = millis();
    while(millis() - startTime < 800 and !lineSensor.seeLine){
        readSensors();
        motorL.setSpeed(33);
        motorR.setSpeed(-30);
    }
    motorL.stop();
    motorR.stop();

    while(!lineSensor.seeLine){
        //left
        startTime = millis();
        while(millis() - startTime < 350 and !lineSensor.seeLine){
            readSensors();
            motorL.setSpeed(-30);
            motorR.setSpeed(-30);
        }

        motorL.stop();
        motorR.stop();

        delay(200);

        //forward
        startTime = millis();
        while(millis() - startTime < 1200 and !lineSensor.seeLine){
            readSensors();
            motorL.setSpeed(33);
            motorR.setSpeed(-30);
        }
        motorL.stop();
        motorR.stop();

        delay(200);

        //right
        startTime = millis();
        while(millis() - startTime < 650 and !lineSensor.seeLine){
            readSensors();
            motorL.setSpeed(30);
            motorR.setSpeed(30);
        }

        motorL.stop();
        motorR.stop();

        delay(200);

        //forward
        startTime = millis();
        while(millis() - startTime < 1200 and !lineSensor.seeLine){
            readSensors();
            motorL.setSpeed(33);
            motorR.setSpeed(-30);
        }
        motorL.stop();
        motorR.stop();

        delay(200);
    }




}


// motorL.stop();
// motorR.stop();
// unsigned int startTime = millis();
//
//
// while (millis() - startTime < 1000 and !lineSensor.seeLine){
//     readSensors();
//     motorL.setSpeed(30);
//     motorR.setSpeed(-25);
//
//     Serial.print(startTime);
//     Serial.print(" ");
//     Serial.print(millis());
//     Serial.print("\n");
// }
// while (millis() - startTime < 3000 and !lineSensor.seeLine){
//     readSensors();
//     motorL.setSpeed(25);
//     motorR.setSpeed(-30);
//
//     Serial.print(startTime);
//     Serial.print(" ");
//     Serial.print(millis());
//     Serial.print("\n");
// }
// while (millis() - startTime < 7000 and !lineSensor.seeLine){
//     readSensors();
//     motorL.setSpeed(30);
//     motorR.setSpeed(-25);
//
//     Serial.print(startTime);
//     Serial.print(" ");
//     Serial.print(millis());
//     Serial.print("\n");
// }
// while (millis() - startTime < 15000 and !lineSensor.seeLine){
//     readSensors();
//     motorL.setSpeed(25);
//     motorR.setSpeed(-30);
//
//     Serial.print(startTime);
//     Serial.print(" ");
//     Serial.print(millis());
//     Serial.print("\n");
// }
// while (millis() - startTime < 31000 and !lineSensor.seeLine){
//     readSensors();
//     motorL.setSpeed(30);
//     motorR.setSpeed(-25);
//
//     Serial.print(startTime);
//     Serial.print(" ");
//     Serial.print(millis());
//     Serial.print("\n");
// }
//
// motorL.stop();
// motorR.stop();
// delay(200);
