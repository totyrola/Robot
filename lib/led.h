/* 
 Authors: Astir Tadrous, Motassembellah Mohamed Bassiouni, Elena Doumon, Raphael Ramanitranja
 Due date: 31 october 2022
 Group: 02
 Description of the program:

    The Led files affect the bicolor Del of the robot. Hence, we can light it up in red, green and amber, or just keep in closed. 

 Material Identification: 

    - Can only be used with DDRB
*/

#pragma once
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

class Led
{
public:
    Led(uint8_t pinCathode, uint8_t pinAnode);
    void setLedRed();
    void setLedGreen();
    void setLedOff();
    void setLedAmber();
    void blinkLedRed();
    void blinkLedGreen();
    void blinkLedAmber();

private:
    uint8_t delRed;
    uint8_t delGreen;
};
