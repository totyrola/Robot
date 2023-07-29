#include "sound.h"
#include <math.h>  

Sound::Sound()
{
    DDRB |= (1 << DDB2) | (1 << DDB3);
    PORTB |= (1 << PB3);
}

void Sound::initVariables(uint8_t note)
{
    noteFrequency_ = uint16_t(440) * (pow(2.0, ((float(note) -69)/ 12)));
    noCyclesOcr0A_ = (FREQUENCY_PROCESSOR_ / (2 * PRESCALER_ * noteFrequency_)) - 1;
}

void Sound::emitSound()
{
    TCNT0 = 0;
    OCR0A = noCyclesOcr0A_;
    TCCR0A = (1 << COM0A0) | (1 << WGM01);
    TCCR0B |= (1 << CS02);
}

void Sound::emitHighSound()
{
    initVariables(81);
    emitSound();
}

void Sound::emitLowSound()
{
    initVariables(45);
    emitSound();
}

void Sound::stopSound()
{
    TCCR0B = 0;
}