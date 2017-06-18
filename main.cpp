#include <Arduino.h>

#include"Motor.h"

void setup() {
    Serial.begin(9600);
    Serial.println("start");

    Motor motorL(6, 11, 9, 1.15);

    while(true) {
        motorL.setSpeed(20);
        Serial.println(motorL.getSpeed());
        delay(10);
    }
}

void loop(){

}
