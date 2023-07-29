#pragma once
#include "motor.h"

class Navigation
{
    public:
        bool zigZagEnd = false;
        Navigation(Motor::Timer timer);
        void setDirectionAdvance();
        void setDirectionReverse();
        void turnRightSlight();
        void turnRightHard();
        void turnRight();
        void turnLeftSlight();
        void turnLeftHard();
        void turnLeft();
        void goStraightSlow();
        void goStraight();
        void goStraightFast();
        void stationnement();
        void zigZag();
        void stop();
        void customSpeed(uint8_t valueA, uint8_t valueB);
        ~Navigation() = default;
        void givePower();
        void giveMaxPower();

    private:
        Motor motor_;
};
