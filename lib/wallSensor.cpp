#include "wallSensor.h"

WallSensor::WallSensor()
{
    DDRA &= ~(1 << DDA6);
}

uint8_t WallSensor::readValue()
{
    uint16_t sum = 0;

    for (int i = 0; i < 300; i++)
    {
        uint8_t value = converter.lecture(PINA6) >> 2;
        sum += value;
    }
    uint32_t average = sum / 300;
    return average;
}

void WallSensor::detectDistance()
{

    uint8_t value = readValue();
    lineSensor.specialFollowLineWallDetection();

    if (stopwatch.detectionAllowed)
    {
        if (value >= 13 && value < 30)
        {
            _delay_ms(150);
            value = readValue();
            if (value > 35)
            {
                numberOfObstacles++;
                motors.stop();
                sound.emitHighSound();
                _delay_ms(1000);
                sound.stopSound();
                distance = CLOSE;
                saveDistance();
            }
            else if (value < 25)
            {
                numberOfObstacles++;
                motors.stop();
                sound.emitLowSound();
                _delay_ms(1000);
                sound.stopSound();
                distance = FAR;
                saveDistance();
            }
        }
    }
}

void WallSensor::saveDistance()
{
    data = distance;
    switch (distance)
    {
    case CLOSE:
        stopwatch.detectionAllowed = false;
        memory.ecriture(writingAddress, data);
        _delay_ms(5);
        writingAddress++;
        stopwatch.startTimerCTC(outOfReadingZone);
        motors.givePower();
        break;

    case FAR:
        stopwatch.detectionAllowed = false;
        memory.ecriture(writingAddress, data);
        _delay_ms(5);
        writingAddress++;
        stopwatch.startTimerCTC(outOfReadingZone);
        motors.givePower();
        break;

    case NO_OBSTACLE:
        memory.ecriture(writingAddress, data);
        _delay_ms(5);
        writingAddress++;
        break;
    }
}

uint8_t WallSensor::getObstaclePosition(uint16_t readingAddress)
{
    uint8_t *position;
    memory.lecture(readingAddress, position);
    return *position;
}

void WallSensor::avoid2Obstacles(uint16_t pathPosition)
{
    uint8_t obstaclePosition = getObstaclePosition(pathPosition);
    switch (pathPosition)
    {
        // si le robot est au dernier obstacle
    case 0:

        if (obstaclePosition == CLOSE)
        {
            // obstacle au point O
            lineSensor.obstacleO = true;
            motors.givePower();
            motors.turnRightSlight(); // on doit le forcer a aller au chemin sans obstacle pis on peut continuer avec followLine
            _delay_ms(1000);
            lineSensor.followLineObstacle2ndCurve();
        }

        else if (obstaclePosition == FAR)
        {
            // obstacle au point N
            lineSensor.obstacleN = true;
            motors.givePower();
            motors.turnLeftSlight();  // on doit le forcer a aller au chemin sans obstacle pis on peut continuer avec followLine
            _delay_ms(1000);
            lineSensor.followLineObstacle2ndCurve();
        }

        break;

    case 1:

        if (obstaclePosition == CLOSE)
        {
            // obstacle au point H ou F
            motors.givePower();
            motors.turnRightSlight(); // on doit le forcer a aller au chemin sans obstacle pis on peut continuer avec followLine
            _delay_ms(1000);
            lineSensor.followLineObstacles();
            lineSensor.adjustSlightlyRight(); // Motasem
            _delay_ms(700); // Motassem
        }

        else if (obstaclePosition == FAR)
        {
            // obstacle au point I ou G
            motors.givePower();
            motors.turnLeftSlight();
            _delay_ms(1000);
            lineSensor.followLineObstacles();
            lineSensor.adjustSlightlyLeft(); // Motasem
            _delay_ms(700); // Motassem

        }

        break;

    case 2:
        if (obstaclePosition == NO_OBSTACLE)
        {
            // NO OBSTACLE IN MIDDLE
            motors.givePower();
            uint16_t timeFollowLine =  lineSensor.followLineCounter.convertTimeToCycles(2300000);
            lineSensor.followLineCounter.startTimerCTC(timeFollowLine);

            while (!(lineSensor.followLineCounter.finishedFollowLine))
            {
                lineSensor.followLine();
            }
            
            motors.stop();
            _delay_ms(1000);
            motors.givePower();
            motors.setDirectionReverse();
            motors.goStraight();
            _delay_ms(1000);
            motors.stop();
            _delay_ms(1000);
            motors.givePower();
            motors.setDirectionAdvance();
            lineSensor.followLineObstacles();
        }

        break;
    }
    lineSensor.followLineCounter.finishedFollowLine = false;
}

void WallSensor::avoid3Obstacles(uint16_t pathPosition)
{
    uint8_t obstaclePosition = getObstaclePosition(pathPosition);
    switch (pathPosition)
    {
        // si le robot est au dernier obstacle
    case 0:

        if (obstaclePosition == CLOSE)
        {
            // obstacle au point N
            lineSensor.obstacleN = true;
            motors.givePower();
            motors.turnRightSlight(); // on doit le forcer a aller au chemin sans obstacle pis on peut continuer avec followLineInitial()
            _delay_ms(1000);
            lineSensor.followLineObstacle2ndCurve();
        }

        else if (obstaclePosition == FAR)
        {
            lineSensor.obstacleO = true;
            // obstacle au point O
            motors.givePower();
            motors.turnLeftSlight(); // on doit le forcer a aller au chemin sans obstacle pis on peut continuer avec followLineInitial()
            _delay_ms(1000);
            lineSensor.followLineObstacle2ndCurve();
        }

        break;

    case 1:

        if (obstaclePosition == CLOSE)
        {
            // obstacle au point K 
            motors.givePower();
            uint16_t timeFollowLine =  lineSensor.followLineCounter.convertTimeToCycles(2400000);
            lineSensor.followLineCounter.startTimerCTC(timeFollowLine);

            while (!(lineSensor.followLineCounter.finishedFollowLine))
            {
                lineSensor.followLine();
            }

            motors.stop();
            _delay_ms(1500);

            motors.givePower();
            motors.turnLeftSlight();
            _delay_ms(1200);
            
            motors.stop();
            _delay_ms(1000);

            motors.givePower();

            while (lineSensor.isStillLost())
            {
                motors.turnRight();
            }

            lineSensor.adjustSlightlyLeft();
            _delay_ms(300);

            // continuer le chemin avec suiveur de ligne a partir d'ici
            lineSensor.followLineObstacles();
        }

        else if (obstaclePosition == FAR)
        {
            // obstacle au point L
            motors.givePower();
            uint16_t timeFollowLine =  lineSensor.followLineCounter.convertTimeToCycles(2400000);
            lineSensor.followLineCounter.startTimerCTC(timeFollowLine);

            while (!(lineSensor.followLineCounter.finishedFollowLine))
            {
                lineSensor.followLine();
            }
            
            motors.stop();
            _delay_ms(1500);

            motors.givePower();
            motors.turnRightSlight();
            _delay_ms(1200);
            
            motors.stop();
            _delay_ms(1000);

            motors.givePower();

            while (lineSensor.isStillLost())
            {
                motors.turnLeft();
            }

            lineSensor.adjustSlightlyRight();
            _delay_ms(300);
            // continuer le chemin avec suiveur de ligne a partir d'ici
            lineSensor.followLineObstacles();
        }

        break;

    case 2:

        if (obstaclePosition == CLOSE)
        {
            // obstacle au point H ou F
            motors.givePower();
            motors.turnRightSlight(); // on doit le forcer a aller au chemin sans obstacle pis on peut continuer avec followLineInitial()
            _delay_ms(1000);
            lineSensor.followLineObstacles();
            motors.stop();
            _delay_ms(1000);

            lineSensor.adjustSlightlyRight();
            _delay_ms(700);
        }

        else if (obstaclePosition == FAR)
        {
            // obstacle au point I ou G
            motors.givePower();
            motors.turnLeftSlight();
            _delay_ms(1000);
            lineSensor.followLineObstacles();
            motors.stop();
            _delay_ms(1000);

            lineSensor.adjustSlightlyLeft();
            _delay_ms(700);
        }

        break;
    }
    lineSensor.followLineCounter.finishedFollowLine = false;
}