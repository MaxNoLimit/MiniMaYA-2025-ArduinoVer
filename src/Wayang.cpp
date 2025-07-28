#include "Wayang.hpp"

/* Example from sita.cpp */

void Wayang::JatayuTakeoff()
{
    moveWhatServo(1, 23, 200);
    moveWhatServo(2, 180, 500);
}

void Wayang::JatayuDive()
{
    moveWhatServo(1, 145, 200);
    moveWhatServo(2, 0, 500);
}

void Wayang::JatayuTalon()
{
    moveWhatServo(1, 0, 200);
    moveWhatServo(2, 90, 500);
}

void Wayang::JatayuDefaultFly()
{
    defaultFaceOrientation();
    moveWhatServo(1, 45, 200);
    moveWhatServo(2, 0, 500);
}

void Wayang::SitaPointToFront()
{
    moveWhatServo(1, 180, 500);
    moveWhatServo(2, 150, 200);
}

void Wayang::SitaLowPointToFront()
{
    moveWhatServo(1, 90, 500);
    moveWhatServo(2, 130, 200);
}

void Wayang::SitaMiddleFront()
{
    moveWhatServo(1, 150, 500);
    moveWhatServo(2, 120, 200);
}

void Wayang::SitaDownFront()
{
    moveWhatServo(1, 0, 500);
    moveWhatServo(2, 121, 200);
}

void Wayang::SitaPointToSelf()
{
    moveWhatServo(2, 60, 200);
    moveWhatServo(1, 180, 500);
    moveWhatServo(2, 100, 200);
}

void Wayang::RahwanaPointToFront()
{
    moveWhatServo(3, 0, 500);
    moveWhatServo(4, 72, 200);
}

void Wayang::RahwanaLowPointToFront()
{
    moveWhatServo(3, 90, 500);
    moveWhatServo(4, 80, 200);
}

void Wayang::RahwanaMiddleFront()
{
    moveWhatServo(3, 0, 500);
}

void Wayang::RahwanaDownFront()
{
    moveWhatServo(3, 180, 500);
    moveWhatServo(4, 87, 200);
}

void Wayang::RahwanaPointToSelf()
{
    moveWhatServo(4, 140, 200);
    moveWhatServo(3, 0, 500);
    moveWhatServo(4, 110, 200);
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
        moveWhatServo(5, 10, 1200);
        // moveWhatServo(5, 0, 400);
    }
    else
    {
        Serial2.println(F("This is Jatayu"));
        moveWhatServo(3, 180, 1200);
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
        moveWhatServo(5, 180, 1200);
    }
    else
    {
        Serial2.println(F("This is Jatayu"));
        moveWhatServo(3, 0, 1200);
    }
}