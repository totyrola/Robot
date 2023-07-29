#pragma once
#define F_CPU 8000000
#include <avr/io.h>
#include <stdio.h>
#include "parcours.h"
#include "communication.h"
#include "wallSensor.h"
#include "navigation.h"
#include "motor.h"

Button whiteButton = Button(INT1, Button::typeEdge::FALLING);
Parcours parcours;
Led led = Led(PB0, PB1);
Motor::Timer wheelTimer = Motor::Timer::TIMER2;
Navigation motors = Navigation(wheelTimer);

ISR(INT1_vect)
{
    _delay_ms(30);
    parcours.nextState(parcours.gState);
    EIFR |= (1 << INTF1);
};

ISR(INT0_vect)
{
    _delay_ms(30);
    parcours.interruptButton.gButtonPressed = true;
    EIFR |= (1 << INTF0);
};

ISR(TIMER1_COMPA_vect)
{
    parcours.wallSensor.stopwatch.detectionAllowed = true;
    parcours.wallSensor.lineSensor.followLineCounter.finishedFollowLine = true;
    parcours.lineSensor.followLineCounter.finishedFollowLine = true;
    parcours.lineSensor.followLineCounter.finishedZigZag = true;
    TCCR1B = 0;
}
int main()
{

    parcours.interruptButton.gButtonPressed = false;

    while(parcours.interruptButton.gButtonPressed == false ) {
        switch (parcours.gState)
        {
        case Parcours::states::A:
            led.setLedGreen();
            break;
        case Parcours::states::B:
            led.setLedRed();
            break;
        case Parcours::states::S:
            led.setLedAmber();
        }
    }

    while(true) //(parcours.gState != Parcours::states::END)
    {
        parcours.behaviourState(parcours.gState);
    }
    return 0;

}

/////////////////////////////////////////// Test readValues moyenne ///////////////////////////
// WallSensor wallSensor;
// LineSensor line;
// Communication communication;

// int main() {
//     while(true) {
//         // uint8_t value = wallSensor.readValue();
//         // line.zigZag();
//         //  uint8_t value = line.numberOfZigs;
//         // communication.transferUart(value);
//         line.followLineObstacle2ndCurve();
//     }

//  }

/////////////////////////////////////////////// Test motors ///////////////////////////
// int main() {
//     Motor::Timer wheelTimer = Motor::Timer::TIMER2;
//     Navigation motors = Navigation(wheelTimer);
//     // motors.setDirectionAdvance();
//     // motors.goStraight();
//     motors.stationnement();
// }

///////////////////////////////// Test write in memory what captor sees //////////////////////////
// Parcours parcours;
// Communication communication;
//  int main () {
//     while(true) {
//         // uint8_t readValues = parcours.wallSensor.readValue(); // devra juste sauvegarder une seule valeur vu on ne fait qu'un appel
//         // communication.transferUart(readValues);
//         uint8_t valueInMemory0 = parcours.wallSensor.getObstaclePosition(0);
//         communication.transferUart(valueInMemory0);
//         uint8_t valueInMemory1 = parcours.wallSensor.getObstaclePosition(1);
//         communication.transferUart(valueInMemory1);
//         uint8_t valueInMemory2 = parcours.wallSensor.getObstaclePosition(2);
//         communication.transferUart(valueInMemory2);
//         uint8_t valueInMemory3 = parcours.getNumberOfObstacles(3);
//         communication.transferUart(valueInMemory3);
//     }

//  }

/////////////////////////////// Test Motassem ////////////////////////////////////

//Parcours test = Parcours();

// ISR(INT0_vect)
// {
//     _delay_ms(30);
//     test.interruptButton.gButtonPressed = true;
// };

// int main()
// {
//     while (true)
//     {
//         test.stateA();

//     }

// }

////////////////////////////////////// Test partie B //////////////////////////////

// Parcours test = Parcours();

// ISR(INT0_vect)
// {
//     _delay_ms(30);
//     test.interruptButton.gButtonPressed = true;
// };

// int main()
// {
//     while (true)
//     {
//         //test.stateA();
//         test.stateB();

//     }

// }

// int main() {
//     Sound sound;
//     while(true) {
//         sound.emitHighSound();
//         _delay_ms(2000);
//        sound.emitLowSound();
//      _delay_ms(2000);
//     }
// }



////////////////////////////////////// Test partie S //////////////////////////////
// Parcours test = Parcours();

// ISR(INT0_vect)
// {
//     _delay_ms(30);
//     test.interruptButton.gButtonPressed = true;
// };

// int main()
// {
//   while(true)  {
//       test.stateS();
//   }
// }

// int main() {
//     setDirectionReverse();
//     customSpeed(170, 170);
//     _delay_ms(950);
//     customSpeed(10, 180);
//     _delay_ms(1500);
//     customSpeed(170, 150);
//     _delay_ms(1200);
//     stop();        
// }