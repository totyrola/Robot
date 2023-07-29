#include "debug.h"

Communication comm;

void print(uint8_t x)
{
    comm.transferUart(x);
}

void printSentence(char sentence[]) 
{   
    uint8_t i, j;
    for ( i = 0; i < 100; i++ ) 
    {
        for ( j=0; j < 20; j++ ) 
        {
            comm.transferUart(sentence[j]);
        }
    }      
}