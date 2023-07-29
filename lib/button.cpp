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


#include "button.h"

Button::Button(uint8_t typeButton, Button::typeEdge typeEdge)
{
    gButtonPressed = false;
    if (typeButton == INT0)
    {
        DDRD &= ~D2_MASK;
        EIMSK |= (1 << INT0);
        typeEdge_ = typeEdge;
        switch (typeEdge_)
        {
            case typeEdge::FALLING:
                EICRA |= (1 << ISC01);
                sei();
                break;
            case typeEdge::RISING:
                EICRA |= (1 << ISC00) | (1 << ISC01);
                sei();
                break;
            case typeEdge::ANY:
                EICRA |= (1 << ISC00);
                sei();
                break;
        };
    }
    else
    {
        DDRD &= ~D3_MASK;
        EIMSK |= (1 << INT1);
        typeEdge_ = typeEdge;
        switch (typeEdge_)
        {
            case typeEdge::FALLING:
                EICRA |= (1 << ISC11);
                sei();
                break;
            case typeEdge::RISING:
                EICRA |= (1 << ISC10) | (1 << ISC11);
                sei();
                break;
            case typeEdge::ANY:
                EICRA |= (1 << ISC10);
                sei();
                break;
        };
    };
}

bool Button::debounceForD2()
{
    bool FIRST_LECTURE = PIND & D2_MASK;
    _delay_ms(30);
    bool SECOND_LECTURE = PIND & D2_MASK;

    return FIRST_LECTURE == SECOND_LECTURE;
}