#include"Motor.h"
#include"LineSensor.h"
#include"SonicSensor.h";

class Robot{
private:
    Motor motorL;
    Motor motorR;
    LineSensor lineSensor;
    SonicSensor sonicSensor;
    void followLine();
    void findLine();
    void barrier();
public:
    void start;
};
