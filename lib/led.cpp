/* 
 Authors: Astir Tadrous, Motassembellah Mohamed Bassiouni, Elena Doumon, Raphael Ramanitranja
 Due date: 31 october 2022
 Group: 02
 Description of the program:

    The Led files affect the bicolor Del of the robot. Hence, we can light it up in red, green and amber, or just keep in closed. 

 Material Identification: 
 
    - Can only be used with DDRB
*/
#include "led.h"

Led::Led(uint8_t pinCathode, uint8_t pinAnode)
{
    DDRB |= (1 << PB0) | (1 << PB1);
    delRed = (1 << pinCathode);
    delGreen = (1 << pinAnode);
}

void Led::setLedRed()
{
    PORTB = delRed;
}

void Led::setLedGreen()
{
    PORTB = delGreen;
}

void Led::setLedOff()
{
    PORTB = ~delRed & ~delGreen;
}

void Led::setLedAmber()
{
    for (uint8_t i = 0; i < 10; i++)
    {
        setLedRed();
        _delay_ms(5);
        setLedGreen();
        _delay_ms(7);
    }
}

void Led::blinkLedRed()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        setLedRed();
        _delay_ms(125);
        setLedOff();
        _delay_ms(125);
    }
    setLedOff();
}

void Led::blinkLedGreen()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        setLedGreen();
        _delay_ms(125);
        setLedOff();
        _delay_ms(125);
    }
    setLedOff();
}

void Led::blinkLedAmber()
{
    for (uint8_t i = 0; i < 7; i++)
    {
        for (uint8_t i = 0; i < 10; i++)
        {
            setLedRed();
            _delay_ms(5);
            setLedGreen();
            _delay_ms(7);
        }
        setLedOff();
        _delay_ms(125);
    }
    setLedOff();
}