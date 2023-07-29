/* 
 Authors: Astir Tadrous, Motassembellah Mohamed Bassiouni, Elena Doumon, Raphael Ramanitranja
 Due date: 31 october 2022
 Group: 02
 Description of the program: The counter cpp works like a stopwatch that uses microseconds
 Material identification (Broches I/O) : PD5 - OC1A is used as the output compare of our timer
*/

#include "counter.h"

Counter::Counter()
{
    // le timer n'est pas lancé
    sei();// ouvrir les interruptions
}

uint16_t Counter::convertTimeToCycles(uint32_t timeUs)
{
    const uint16_t FREQUENCY_HZ_SLOWED = FREQUENCY_HZ_ / PRESCALER_;
    const uint32_t PERIOD_US = uint32_t(1000000) / FREQUENCY_HZ_SLOWED;
    uint16_t cycles = timeUs / PERIOD_US;

    return cycles;
}

void Counter::startTimerCTC(uint16_t cycles)
{
    TCNT1 = 0;
    OCR1A = cycles;
    TCCR1A = 0;
    // mode CTC
    TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM12);
    TCCR1C = 0;
    TIMSK1 |= (1 << OCIE1A);
}

void Counter::startTimerPWM(uint16_t cycles)
{
    TCNT1 = 0;
    OCR1A = cycles;
    TCCR1A = 0;
    // mode PWM, Phase and Frequency Correct
    TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM10);
    TCCR1C = 0;
    TIMSK1 |= (1 << OCIE1A);
}

void Counter::startTimerFastPWM(uint16_t cycles)
{
    TCNT1 = 0;
    OCR1A = cycles;
    TCCR1A = 0;
    // mode Fast PWM
    TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM10) | (1 << WGM11) | (1 << WGM12);
    TCCR1C = 0;
    TIMSK1 |= (1 << OCIE1A);
}
