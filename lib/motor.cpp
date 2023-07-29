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

#include "motor.h"
const uint8_t DDRB3_B4_B5_B6_OUT = (1 << DDB4) | (1 << DDB3) | (1 << DDB5) | (1 << DDB6);
const uint8_t ENABLE_PWM_B3_B4_TIMER0 = (1 << PB3) | (1 << PB4);
const uint8_t DDRD4_D5_D6_D7_OUT = (1 << DDD6) | (1 << DDD7) | (1 << DDD5) | (1 << DDD4);
const uint8_t ENABLE_PWM_D6_D7_TIMER2 = (1 << PD6) | (1 << PD7);
const uint8_t DIRECTION_ADVANCE_TIMER0 = ~(1 << PB5) & ~(1 << PB6);
const uint8_t DIRECTION_REVERSE_TIMER0 = (1 << PB5) | (1 << PB6);
const uint8_t DIRECTION_ADVANCE_TIMER2 = ~(1 << PD4) & ~(1 << PD5);
const uint8_t DIRECTION_REVERSE_TIMER2 = (1 << PD4) | (1 << PD5);


Motor::Motor(Motor::Timer timer)
{

    timer_ = timer;
    switch (timer_)
    {
        case Timer::TIMER0:
        {

            TCNT0 = 0;
            TCCR0A |= (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
            TCCR0B |= (1 << CS01);
            DDRB |= DDRB3_B4_B5_B6_OUT;
            PORTB |= ENABLE_PWM_B3_B4_TIMER0;
            break;
        }

        case Timer::TIMER2:
        {

            TCNT2 = 0;
            TCCR2A |= (1 << WGM20) | (1 << COM2A1) | (1 << COM2B1);
            TCCR2B |= (1 << CS21);
            DDRD |= DDRD4_D5_D6_D7_OUT;
            PORTD |= ENABLE_PWM_D6_D7_TIMER2;
            break;
        }
    }
}

void Motor::adjustDirection(Direction direction)
{

    direction_ = direction;

    if (timer_ == Timer::TIMER2)
    {
        switch (direction_)
        {
            case Direction::ADVANCE:
                PORTD &= DIRECTION_ADVANCE_TIMER2;
                break;

            case Direction::REVERSE:
                PORTD |= DIRECTION_REVERSE_TIMER2;
                break;
        }
    }

    else
    {
        switch (direction_)
        {
            case Direction::ADVANCE:
                PORTB &= DIRECTION_ADVANCE_TIMER0;
                break;

            case Direction::REVERSE:
                PORTB |= DIRECTION_REVERSE_TIMER0;
                break;
        }
    }
}

void Motor::ajustPWMPourcentage(uint8_t valueA, uint8_t valueB)
{
    if (timer_ == Timer::TIMER0)
    {
        OCR0A = valueA;
        OCR0B = valueB;
    }

    else
    {
        OCR2A = valueA;
        OCR2B = valueB;
    }
}

