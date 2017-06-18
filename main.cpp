#include <Arduino.h>

#include"Motor.h"

void setup() {
    Serial.begin(57600);
    Serial.println("start");

    Motor motorL(6, 11, 9, 1.15);

    while(true) {
        motorL.setSpeed(35);

        delay(5);
    }
}

void loop(){

}
