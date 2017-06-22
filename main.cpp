#include <Arduino.h>

#include "Robot.h"

void setup() {
    Serial.begin(57600);
    Serial.println("start");

    Robot lineFollow;

    while(true) {
        lineFollow.start();
    }
}

void loop(){

}
