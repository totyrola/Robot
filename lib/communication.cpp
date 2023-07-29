/* 
 Authors: Astir Tadrous, Motassembellah Mohamed Bassiouni, Elena Doumon, Raphael Ramanitranja
 Due date: 31 october 2022
 Group: 02
 Description of the program:

    From outside the class "Communication" created in the file "Communication.h", "Communication.cpp" implements the functions of "Communication.h" 
    to have a functional RS232 protocol. The constructor initializes the UART. Then, we have implemented a function that transfers only one character by using UART.
    Finally, we decided to use this function in another one by iterating on it to help the client transfer all the data, not just one character. 

 Material used:
    - PC
    - SerieViaUSB
    - Printer Cable 
    - Microcontroller
*/

#include "communication.h"

Communication::Communication()
{
    UBRR0H = 0; 
    UBRR0L = 0xCF; 
    UCSR0B = (1 << TXEN0)| (1 << RXEN0); 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void Communication::transferUart(uint8_t data)
{
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

uint8_t Communication::receiveData(){
    while (!(UCSR0A & (1 << RXC0))) {

    }

    return UDR0;
    
}

void Communication::transferUart(uint8_t length, uint8_t readSentence[])
{
    uint8_t currentAdress = 0;
    if (length <= 255)
    {
        while (currentAdress != length)
        {
            transferUart(readSentence[currentAdress]);
            currentAdress++;
        }
    }
}
