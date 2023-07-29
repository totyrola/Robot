#include "lineSensor.h"
#include <util/delay.h>

LineSensor::LineSensor()
{
    DDRA &= ~SENSOR_EXTREME_LEFT & ~SENSOR_LEFT & ~SENSOR_MIDDLE & ~SENSOR_RIGHT & ~SENSOR_EXTREME_RIGHT;
};

bool LineSensor::detectBlackLine(const uint8_t brocheAnMask)
{
    bool detected = PINA & brocheAnMask;
    return detected;
}

void LineSensor::continueStraight()
{
    navigation.goStraight();
}

void LineSensor::adjustSlightlyRight()
{
    navigation.turnRightSlight();
}

void LineSensor::adjustHardlyRight()
{
    navigation.turnRightHard();
}

void LineSensor::adjustSlightlyLeft()
{
    navigation.turnLeftSlight();
}

void LineSensor::adjustHardlyLeft()
{
    navigation.turnLeftHard();
}

void LineSensor::stopAtSign()
{
    navigation.stop();
}

void LineSensor::takeYourLeft()
{
    navigation.stop();
    _delay_ms(400);
    navigation.givePower();
    while (isStillLost())
    {
        navigation.turnLeft();
    }

    adjustSlightlyRight();
    _delay_ms(300);
    counter++;
}

void LineSensor::takeYourRight()
{
    navigation.stop();
    _delay_ms(400);
    navigation.givePower();
    while (isStillLost())
    {
        navigation.turnRight();
    }

    adjustSlightlyLeft();
    _delay_ms(300);
}

void LineSensor::zigZag()
{
    
    bool extremeRightDetection = detectBlackLine(SENSOR_EXTREME_RIGHT);
    bool extremeLeftDetection = detectBlackLine(SENSOR_EXTREME_LEFT);
    if (extremeLeftDetection && !zigZagLastDetectedLeft) 
    {
        zigZagLastDetectedLeft = true;
        navigation.givePower();
        navigation.turnRightSlight();
        numberOfZigZag++;
    }

    else if (extremeRightDetection && zigZagLastDetectedLeft)
    {
        zigZagLastDetectedLeft = false;
        navigation.givePower();
        navigation.turnLeftSlight();
    }

}

void LineSensor::specialFollowLineWallDetection()
{
    bool extremeLeftDetection = detectBlackLine(SENSOR_EXTREME_LEFT);
    bool leftDetection = detectBlackLine(SENSOR_LEFT);
    bool middleDetection = detectBlackLine(SENSOR_MIDDLE);
    bool rightDetection = detectBlackLine(SENSOR_RIGHT);
    bool extremeRightDetection = detectBlackLine(SENSOR_EXTREME_RIGHT);

    bool completelyLeft = extremeRightDetection && !(rightDetection) && !(extremeLeftDetection) && !(leftDetection) && !(middleDetection);
    bool left = rightDetection && !(extremeLeftDetection) && !(leftDetection);
    bool middle = !(extremeLeftDetection) && !(leftDetection) && !(rightDetection) && middleDetection && !(extremeRightDetection);
    bool right = leftDetection && !(extremeRightDetection) && !(rightDetection);
    bool completelyRight = extremeLeftDetection && !(extremeRightDetection) && !(rightDetection) && !(middleDetection) && !(left);

    
    bool twoExtremes = extremeRightDetection && extremeLeftDetection;

    
    if (middle)
    {
        if (!(followLineCounter.followLineEndA))
        {
           continueStraight(); 
        }
    }

    else if (left)
    {
        if (!(followLineCounter.followLineEndA))
        {
            adjustSlightlyRight();
        }
    }

    else if (completelyLeft)
    {
        if (!(followLineCounter.followLineEndA))
        {
            adjustHardlyRight();
        }
    }

    else if (right)
    {
        if (!(followLineCounter.followLineEndA))
        {
            adjustSlightlyLeft();
        }
    }

    else if (completelyRight)
    {
        if (!(followLineCounter.followLineEndA))
        {   
            adjustHardlyLeft();
        }
    }

    else if (twoExtremes)
    {
        navigation.stop();
        _delay_ms(2000);
        followLineCounter.followLineEndA = true;
    }
}







void LineSensor::followLine()
{
    bool extremeLeftDetection = detectBlackLine(SENSOR_EXTREME_LEFT);
    bool leftDetection = detectBlackLine(SENSOR_LEFT);
    bool middleDetection = detectBlackLine(SENSOR_MIDDLE);
    bool rightDetection = detectBlackLine(SENSOR_RIGHT);
    bool extremeRightDetection = detectBlackLine(SENSOR_EXTREME_RIGHT);

    bool completelyLeft = extremeRightDetection && !(rightDetection) && !(extremeLeftDetection) && !(leftDetection) && !(middleDetection);
    bool left = rightDetection && !(extremeLeftDetection) && !(leftDetection);
    bool middle = !(extremeLeftDetection) && !(leftDetection) && !(rightDetection) && middleDetection && !(extremeRightDetection);
    bool right = leftDetection && !(extremeRightDetection) && !(rightDetection);
    bool completelyRight = extremeLeftDetection && !(extremeRightDetection) && !(rightDetection) && !(middleDetection) && !(left);

    bool leftMiddleRight = leftDetection && middleDetection && rightDetection;
    bool twoExtremes = extremeRightDetection && extremeLeftDetection;

    bool turnPointLeft = extremeLeftDetection && leftDetection && middleDetection;
    bool turnPointRight = extremeRightDetection && rightDetection && middleDetection;

    bool lost = !(extremeLeftDetection) && !(leftDetection) && !(middleDetection) && !(rightDetection) && !(extremeRightDetection);

    if (middle)
    {
        continueStraight();
    }

    else if (left)
    {
        adjustSlightlyRight();
    }

    else if (completelyLeft)
    {
        adjustHardlyRight();
    }

    else if (right)
    {
        adjustSlightlyLeft();
    }

    else if (completelyRight)
    {
        adjustHardlyLeft();
    }

    else if (turnPointLeft)
    {
        if (counter < 2)
        {
            takeYourLeft();
            heyStop = false;
        }
        else
        {
            navigation.stop();
        }
    }

    else if (lost)
    {
        if (!heyStop && !stateSfollowLine)
        {
            takeYourLeft();
        }

        else if (heyStop || stateSfollowLine)
        {
            navigation.stop();
            okayStoped = true;
        }
    }

    else if (leftMiddleRight)
    {
        if (counter == 2)
        {
            navigation.stop();
        }

        heyStop = true;
    }
}

void LineSensor::followLineAfterObstacles()
{
    bool extremeLeftDetection = detectBlackLine(SENSOR_EXTREME_LEFT);
    bool leftDetection = detectBlackLine(SENSOR_LEFT);
    bool middleDetection = detectBlackLine(SENSOR_MIDDLE);
    bool rightDetection = detectBlackLine(SENSOR_RIGHT);
    bool extremeRightDetection = detectBlackLine(SENSOR_EXTREME_RIGHT);

    bool completelyLeft = extremeRightDetection && !(rightDetection) && !(extremeLeftDetection) && !(leftDetection) && !(middleDetection);
    bool left = rightDetection && !(extremeLeftDetection) && !(leftDetection);
    bool middle = !(extremeLeftDetection) && !(leftDetection) && !(rightDetection) && middleDetection && !(extremeRightDetection);
    bool right = leftDetection && !(extremeRightDetection) && !(rightDetection);
    bool completelyRight = extremeLeftDetection && !(extremeRightDetection) && !(rightDetection) && !(middleDetection) && !(left);

    bool turnPointRight = extremeRightDetection && rightDetection && middleDetection;

    bool lost = !(extremeLeftDetection) && !(leftDetection) && !(middleDetection) && !(rightDetection) && !(extremeRightDetection);

    if (middle)
    {
        continueStraight();
    }

    else if (left)
    {
        adjustSlightlyRight();
    }

    else if (completelyLeft)
    {
        adjustHardlyRight();
    }

    else if (right)
    {
        adjustSlightlyLeft();
    }

    else if (completelyRight)
    {
        adjustHardlyLeft();
    }

    else if (lost)
    {
        takeYourRight();
    }

    else if (turnPointRight)
    {
        takeYourRight();
    }
}

void LineSensor::followLineObstacles()
{

    while (!(isAtStopSignObstacle()))
    {

        bool extremeLeftDetection = detectBlackLine(SENSOR_EXTREME_LEFT);
        bool leftDetection = detectBlackLine(SENSOR_LEFT);
        bool middleDetection = detectBlackLine(SENSOR_MIDDLE);
        bool rightDetection = detectBlackLine(SENSOR_RIGHT);
        bool extremeRightDetection = detectBlackLine(SENSOR_EXTREME_RIGHT);

        bool completelyLeft = extremeRightDetection && !(rightDetection) && !(extremeLeftDetection) && !(leftDetection) && !(middleDetection);
        bool left = rightDetection && !(extremeLeftDetection) && !(leftDetection);
        bool middle = !(extremeLeftDetection) && !(leftDetection) && !(rightDetection) && middleDetection && !(extremeRightDetection);
        bool right = leftDetection && !(extremeRightDetection) && !(rightDetection);
        bool completelyRight = extremeLeftDetection && !(extremeRightDetection) && !(rightDetection) && !(middleDetection) && !(left);

        bool leftMiddleRight = leftDetection && middleDetection && rightDetection;
        bool twoExtremes = extremeRightDetection && extremeLeftDetection;

        bool turnPointLeft = extremeLeftDetection && leftDetection && middleDetection;
        bool turnPointRight = extremeRightDetection && rightDetection && middleDetection;

        bool lost = !(extremeLeftDetection) && !(leftDetection) && !(middleDetection) && !(rightDetection) && !(extremeRightDetection);

        if (middle)
        {
            continueStraight();
        }

        else if (left)
        {
            adjustSlightlyRight();
        }

        else if (completelyLeft)
        {
            adjustHardlyRight();
        }

        else if (right)
        {
            adjustSlightlyLeft();
        }

        else if (completelyRight)
        {
            adjustHardlyLeft();
        }

        else if (lost)
        {
            navigation.stop();
        }
    }
}

void LineSensor::followLineObstacle2ndCurve()
{

    while (!(isAtStopSignObstacle()))
    {

        bool extremeLeftDetection = detectBlackLine(SENSOR_EXTREME_LEFT);
        bool leftDetection = detectBlackLine(SENSOR_LEFT);
        bool middleDetection = detectBlackLine(SENSOR_MIDDLE);
        bool rightDetection = detectBlackLine(SENSOR_RIGHT);
        bool extremeRightDetection = detectBlackLine(SENSOR_EXTREME_RIGHT);

        bool completelyLeft = extremeRightDetection && !(rightDetection) && !(extremeLeftDetection) && !(leftDetection) && !(middleDetection);
        bool left = rightDetection && !(extremeLeftDetection) && !(leftDetection);
        bool middle = !(extremeLeftDetection) && !(leftDetection) && !(rightDetection) && middleDetection && !(extremeRightDetection);
        bool right = leftDetection && !(extremeRightDetection) && !(rightDetection);
        bool completelyRight = extremeLeftDetection && !(extremeRightDetection) && !(rightDetection) && !(middleDetection) && !(left);

        bool leftMiddleRight = leftDetection && middleDetection && rightDetection;
        bool twoExtremes = extremeRightDetection && extremeLeftDetection;

        bool turnPointLeft = extremeLeftDetection && leftDetection && middleDetection;
        bool turnPointRight = extremeRightDetection && rightDetection && middleDetection;

        bool lost = !(extremeLeftDetection) && !(leftDetection) && !(middleDetection) && !(rightDetection) && !(extremeRightDetection);

        if (middle)
        {
            // continueStraight();
            navigation.customSpeed(130, 130);
        }

        else if (left)
        {
            // adjustSlightlyRight();
            navigation.customSpeed(70,140);
        }

        else if (completelyLeft)
        {
            // adjustHardlyRight();
            navigation.customSpeed(40,220);
        }

        else if (right)
        {
            // adjustSlightlyLeft();
            navigation.customSpeed(140,70);

        }

        else if (completelyRight)
        {
            navigation.customSpeed(220, 40);
        }

        else if (lost)
        {
            if(obstacleO)
            {
                navigation.turnRightSlight();
            }
            else if(obstacleN)
            {
                navigation.turnLeftSlight();
            }
        }
    }
    navigation.stop();
    _delay_ms(1000);
}

bool LineSensor::isAtStopSignObstacle()
{
    bool middleDetection = detectBlackLine(SENSOR_MIDDLE);
    bool extremeRightDetection = detectBlackLine(SENSOR_EXTREME_RIGHT);
    bool extremeLeftDetection = detectBlackLine(SENSOR_EXTREME_LEFT);
    bool rightDetection = detectBlackLine(SENSOR_RIGHT);
    bool leftDetection = detectBlackLine(SENSOR_LEFT);

    bool twoExtremes = extremeRightDetection && extremeLeftDetection;

    return ((middleDetection && rightDetection && extremeRightDetection) || (middleDetection && leftDetection && extremeLeftDetection) || twoExtremes);
};

bool LineSensor::isAtStopSignNoObstacle()
{
    bool extremeRightDetection = detectBlackLine(SENSOR_EXTREME_RIGHT);
    bool extremeLeftDetection = detectBlackLine(SENSOR_EXTREME_LEFT);

    if (extremeRightDetection && extremeLeftDetection) {
        return true;
    }

    bool middleDetection = detectBlackLine(SENSOR_MIDDLE);
    bool rightDetection = detectBlackLine(SENSOR_RIGHT);
    bool leftDetection = detectBlackLine(SENSOR_LEFT);

    bool leftMiddleRight = leftDetection && middleDetection && rightDetection;
    bool twoExtremes = extremeRightDetection && extremeLeftDetection;

    if (counter < 2)
    {
        return twoExtremes;
    }

    else
    {
        return (twoExtremes || leftMiddleRight);
    }
};

bool LineSensor::isAtFinalStop()
{
    bool middleDetection = detectBlackLine(SENSOR_MIDDLE);
    bool extremeRightDetection = detectBlackLine(SENSOR_EXTREME_RIGHT);
    bool extremeLeftDetection = detectBlackLine(SENSOR_EXTREME_LEFT);
    bool rightDetection = detectBlackLine(SENSOR_RIGHT);
    bool leftDetection = detectBlackLine(SENSOR_LEFT);

    bool leftMiddleRight = leftDetection && middleDetection && rightDetection;
    bool twoExtremes = extremeRightDetection && extremeLeftDetection;
    return ((middleDetection && rightDetection && extremeRightDetection && leftDetection && extremeLeftDetection));
};

bool LineSensor::isStillLost()
{
    bool middleDetection = detectBlackLine(SENSOR_MIDDLE);
    bool extremeRightDetection = detectBlackLine(SENSOR_EXTREME_RIGHT);
    bool extremeLeftDetection = detectBlackLine(SENSOR_EXTREME_LEFT);
    bool rightDetection = detectBlackLine(SENSOR_RIGHT);
    bool leftDetection = detectBlackLine(SENSOR_LEFT);

    return (!(middleDetection || rightDetection || extremeRightDetection || leftDetection || extremeLeftDetection));

}

bool LineSensor::lost() {
    bool middleDetection = detectBlackLine(SENSOR_MIDDLE);
    bool extremeRightDetection = detectBlackLine(SENSOR_EXTREME_RIGHT);
    bool extremeLeftDetection = detectBlackLine(SENSOR_EXTREME_LEFT);
    bool rightDetection = detectBlackLine(SENSOR_RIGHT);
    bool leftDetection = detectBlackLine(SENSOR_LEFT);

    return (!middleDetection && !rightDetection && !extremeRightDetection && !leftDetection && !extremeLeftDetection);
}