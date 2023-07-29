/*
 Authors: Astir Tadrous, Motassembellah Mohamed Bassiouni, Elena Doumon, Raphael Ramanitranja
 Due date: 31 october 2022
 Group: 02
 Description of the program:

    Communication is something crucial in any embedded system. The file "Communication.h" creates a class that could be used in object oriented programs to guarantee
    the presence of a protocol of communication (RS232). The class contains a contructor, a function to transfer one character using UART, and another one that tranfers
    all the data.

 Material used:
    - PC
    - SerieViaUSB
    - Printer Cable
    - Microcontroller
*/
#pragma once
#define F_CPU 8000000
#include <avr/io.h>

class Communication
{
public:
    Communication();
    void transferUart(uint8_t data);
    void transferUart(uint8_t length, uint8_t readSentence[]);
    uint8_t receiveData();
};
