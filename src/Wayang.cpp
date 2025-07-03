#include "Wayang.hpp"

/* Example from sita.cpp */

// takes 700 ms
void Wayang::pointToFront()
{
    moveWhatServo(1, 10, 500);
    moveWhatServo(2, 40, 200);
}
void Wayang::lowPointFront()
{
}
void Wayang::middleFront()
{
}
void Wayang::downFront()
{
}
void Wayang::pointToSelf()
{
}

void Wayang::defaultHandPosition()
{
    // check if this RahwanaSita
    if (getServoPin5() != 0)
    {
        // Rahwana horizontal servo
        moveWhatServo(4, 80, 200);
        // Rahwana vertical servo
        moveWhatServo(3, 80, 500);

        // Sita horizontal servo
        moveWhatServo(2, 80, 200);
        // Sita vertical servo
        moveWhatServo(1, 80, 500);
    }
}

void Wayang::flick()
{
    // is this wayang RahwanaSita
    /* debug */
    Serial2.println("\nSelected pin to spin: " + String(getServoPin5()) + "\n");
    if (getServoPin5() != 0)
    {
        Serial2.println(F("This is RahwanaSita"));
        moveWhatServo(5, 180, 1000);
    }
    else
    {
        Serial2.println(F("This is Jatayu"));
        moveWhatServo(3, 180, 1000);
    }
}

void Wayang::defaultFaceOrientation()
{
    // is this wayang RahwanaSita
    /* debug */
    Serial2.println("\nSelected pin to spin: " + String(getServoPin5()) + "\n");
    if (getServoPin5() != 0)
    {
        Serial2.println(F("This is RahwanaSita"));
        moveWhatServo(5, 0, 1000);
    }
    else
    {
        Serial2.println(F("This is Jatayu"));
        moveWhatServo(3, 0, 1000);
    }
}