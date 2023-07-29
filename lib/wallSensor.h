#pragma once
#define F_CPU 8000000
#include "communication.h"
#include "can.h"
#include "memoire_24.h"
#include "motor.h"
#include "sound.h"
#include "navigation.h"
#include "lineSensor.h"
#include "counter.h"

class WallSensor
{

public:
    enum Distance
    {

        CLOSE = 0,
        FAR = 1,
        NO_OBSTACLE = 2,

    };

    WallSensor();
    ~WallSensor() = default;
    uint8_t readValue();
    void detectDistance();
    void saveDistance();
    Distance distance;
    uint8_t getObstaclePosition(uint16_t readingAddress);
    void avoid2Obstacles(uint16_t pathPosition);
    void avoid3Obstacles(uint16_t pathPosition);
    Counter stopwatch;
    LineSensor lineSensor;
    uint16_t outOfReadingZone = stopwatch.convertTimeToCycles(2000000);
    uint8_t numberOfObstacles;


private:
    uint8_t data;
    uint16_t writingAddress = 0;
    Motor::Timer wheelTimer = Motor::Timer::TIMER2;
    Navigation motors = Navigation(wheelTimer);
    can converter;
    Communication communication;
    Memoire24CXXX memory;
    Sound sound;
};
