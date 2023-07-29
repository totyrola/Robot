/* 
 Authors: Astir Tadrous, Motassembellah Mohamed Bassiouni, Elena Doumon, Raphael Ramanitranja
 Due date: 31 october 2022
 Group: 02
 Description of the program:

    The Led files affect the bicolor Del of the robot. Hence, we can light it up in red, green and amber, or just keep in closed. 
    The button files 
    The functions contained in the button files let us choose which button will be used in our interrupt functions as well as the choice of the 
    edge used when the button is pressed to trigger the interruption such as the rising edge, the falling edge or both.

 Material Identification: 
 
    - Used with DDRD in entry mode
*/


#pragma once
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

class Button
{
public:
    enum class typeEdge
    {
        ANY,
        FALLING,
        RISING
    };
    
    Button(uint8_t typeButton, Button::typeEdge typeEdge);

    bool debounceForD2();
    volatile bool gButtonPressed;

private:
    uint8_t button_;
    typeEdge typeEdge_;
    const uint8_t D2_MASK = (1 << PD2);
    const uint8_t D3_MASK = (1 << PD3);
    bool FIRST_LECTURE;
    bool SECOND_LECTURE;
};