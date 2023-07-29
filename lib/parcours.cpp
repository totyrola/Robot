#include "parcours.h"

Parcours::Parcours() {}

void Parcours::behaviourState(uint8_t state)
{
    // if (interruptButton.gButtonPressed == false) {
    //     return;
    // }

    switch (state)
    {
        case A:
            // interruptButton.gButtonPressed = false;
            stateA();
            break;
        case B:
            // interruptButton.gButtonPressed = false;
            stateB();
            break;
        case S:
            // interruptButton.gButtonPressed = false;
            stateS();
            break;
        case END:
            break;
    }
}

void Parcours::nextState(uint8_t state)
{
    switch (state)
    {
        case A:
            gState = B;
            break;
        case B:
            gState = S;
            break;
        case S:
            gState = A;
            break;
    }
};

uint8_t Parcours::getNumberOfObstacles(uint16_t readingAddress)
{
    uint8_t *number;
    memory.lecture(readingAddress, number);
    return *number;
}

void Parcours::stateA()
{
    // led.setLedGreen();

    // if (interruptButton.gButtonPressed)
    // {
        led.blinkLedGreen();
        lineSensor.continueStraight();
        _delay_ms(700);

        while (true)
        {
            if (wallSensor.lineSensor.followLineCounter.followLineEndA)
            {
                break;
            }

            wallSensor.detectDistance();
            
        }

        navigation.stop();
        _delay_ms(2000);

        if (wallSensor.numberOfObstacles == 2)
        {
            wallSensor.distance = wallSensor.NO_OBSTACLE;
            wallSensor.saveDistance();
        }

        memory.ecriture(3, wallSensor.numberOfObstacles);
        _delay_ms(5);
        
        
        //gState = states::END;

        // interruptButton.gButtonPressed = false;
    // }
}

void Parcours::stateB()
{
    // led.setLedRed();
    // if (interruptButton.gButtonPressed)
    // {
        led.blinkLedRed();
        lineSensor.continueStraight();
        _delay_ms(700);
        while (!(lineSensor.isAtStopSignNoObstacle()))
        {
            lineSensor.followLine();
        }

        if (numberOfObstacles == 2)
        {
            wallSensor.avoid2Obstacles(1);
            wallSensor.avoid2Obstacles(2);
            wallSensor.avoid2Obstacles(0);
        }
        else if (numberOfObstacles == 3)
        {
            wallSensor.avoid3Obstacles(2);
            wallSensor.avoid3Obstacles(1);
            wallSensor.avoid3Obstacles(0);
        };

        navigation.givePower();
        lineSensor.continueStraight();
        _delay_ms(700);

        while (!(lineSensor.isAtFinalStop()))
        {
            lineSensor.followLineAfterObstacles();
        }

        navigation.stop();
        _delay_ms(2000);

        gState = states::S;
        // interruptButton.gButtonPressed = false;
    // }
}

void Parcours::stateS()
{
    // led.setLedAmber();
    // if (interruptButton.gButtonPressed)
    // {
        lineSensor.followLineCounter.finishedFollowLine = false;
        lineSensor.followLineCounter.finishedZigZag = false;
        led.blinkLedAmber();
        navigation.customSpeed(150,150);
        _delay_ms(700);
        uint16_t timeFollowLine =  lineSensor.followLineCounter.convertTimeToCycles(3500000);
        lineSensor.followLineCounter.startTimerCTC(timeFollowLine);
        while (!(lineSensor.followLineCounter.finishedFollowLine))
        {
            lineSensor.followLine();
        }
        //REBONDIR ENTRE UW ET VX TILL Ws
        navigation.turnRightSlight();
        _delay_ms(500);
       
       
        while(lineSensor.numberOfZigZag < 3)
        {
            lineSensor.zigZag();
        }
        lineSensor.adjustSlightlyRight();
        led.setLedRed();
        _delay_ms(2200);
        led.setLedOff();
        
        lineSensor.stateSfollowLine = true;
        while(true) {
            if(lineSensor.lost()) {
                led.setLedGreen();
                _delay_ms(70);
                if(lineSensor.lost()) {
                    break;
                }
                
            }
            lineSensor.followLine();
        }
        navigation.stop();
        _delay_ms(2000);
        
        //STATIONNEMENT
        navigation.stationnement();

        //SON AIGU DECLARE LA FIN DU PARCOURS
        piezo.emitHighSound();
        _delay_ms(1000);
        piezo.stopSound();

        // interruptButton.gButtonPressed = false;
        lineSensor.followLineCounter.finishedFollowLine = false;
        //gState = states::END;
    // }
}
