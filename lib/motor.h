/*
 Authors: Astir Tadrous, Motassembellah Mohamed Bassiouni, Elena Doumon, Raphael Ramanitranja
 Due date: 31 october 2022
 Group: 02
 Description of the program:

    The motor series controls the motors of the robot but does the basic movements. It adjusts the direction of the motors and their speed,
    whilst using either the timer 0 or timer 2.

 Material Identification: When using timer 0, PB5 and PB6 are used to control the direction of the motors
                          When using timer 2, PD4 and PD5 are used to control the direction of the motors
*/

#pragma once
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

class Motor
{
public:
    enum class Timer
    {
        TIMER0,
        TIMER2
    };

    enum class Direction
    {
        ADVANCE,
        REVERSE
    };

    Motor(Timer timer);
    void adjustDirection(Direction direction);
    void ajustPWMPourcentage(uint8_t valueA, uint8_t valueB);
    ~Motor() = default;

private:
    Timer timer_;
    Direction direction_;
};