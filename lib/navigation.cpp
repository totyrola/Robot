/*

*/

#include "navigation.h"

Navigation::Navigation(Motor::Timer timer) : motor_(Motor(timer)) {}

void Navigation::setDirectionAdvance()
{
    Motor::Direction direction = Motor::Direction::ADVANCE;
    motor_.adjustDirection(direction);
}

void Navigation::setDirectionReverse()
{
    Motor::Direction direction = Motor::Direction::REVERSE;
    motor_.adjustDirection(direction);
}

void Navigation::turnLeftSlight()
{
    motor_.ajustPWMPourcentage(130, 50);
}

void Navigation::turnLeftHard()
{
    motor_.ajustPWMPourcentage(200, 50);
}

void Navigation::turnLeft()
{
    motor_.ajustPWMPourcentage(150, 50);
}

void Navigation::turnRightSlight()
{
    motor_.ajustPWMPourcentage(70, 130);
}

void Navigation::turnRightHard()
{
    motor_.ajustPWMPourcentage(50, 200);
}

void Navigation::turnRight()
{
    motor_.ajustPWMPourcentage(70, 150);
}

void Navigation::goStraightSlow()
{
    motor_.ajustPWMPourcentage(250, 250);
    _delay_ms(20);
    motor_.ajustPWMPourcentage(120, 120);
}

void Navigation::goStraight()
{
    motor_.ajustPWMPourcentage(160, 160);
}

void Navigation::goStraightFast()
{
    motor_.ajustPWMPourcentage(210, 210);
}
void Navigation::customSpeed(uint8_t valueA, uint8_t valueB) {
    motor_.ajustPWMPourcentage(valueA, valueB);
}

void Navigation::stationnement()
{
    setDirectionReverse();

    givePower();
    customSpeed(200, 200);
    _delay_ms(700);

    stop();
    _delay_ms(500);

    givePower();
    customSpeed(10, 140);
    _delay_ms(1750);

    stop();
    _delay_ms(500);

    givePower();
    customSpeed(150, 150);
    _delay_ms(1800);
    stop();        
}

void Navigation::zigZag() {
    setDirectionAdvance();
    goStraight();
    _delay_ms(2000);
    turnLeftSlight();
    _delay_ms(1000);
    for (int i = 0 ; i < 6; i++) {
        turnRightSlight();
        _delay_ms(1000);
        turnLeftSlight();
        _delay_ms(1000);
    }
    zigZagEnd = true;
}

void Navigation::stop()
{
    motor_.ajustPWMPourcentage(0, 0);
}

void Navigation::givePower()
{
    motor_.ajustPWMPourcentage(230, 230);
    _delay_ms(20);
}

void Navigation::giveMaxPower()
{
    motor_.ajustPWMPourcentage(250, 250);
    _delay_ms(20);
}