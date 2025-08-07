#include "Wayang.hpp"

/* Example from sita.cpp */

// Takes 700 ms
void Wayang::JatayuTakeoff()
{
    moveWhatServoWithBitBang(1, 23, 200);
    moveWhatServoWithBitBang(2, 180, 500);
}

// Takes 700 ms
void Wayang::JatayuDive()
{
    moveWhatServoWithBitBang(1, 145, 200);
    moveWhatServoWithBitBang(2, 0, 500);
}

// Takes 700 ms
void Wayang::JatayuTalon()
{
    moveWhatServoWithBitBang(1, 0, 200);
    moveWhatServoWithBitBang(2, 90, 500);
}

// Takes 700 ms
void Wayang::JatayuAttack()
{
    moveWhatServoWithBitBang(1, 90, 350);
    moveWhatServoWithBitBang(1, 45, 350);
}

// Takes 700 ms
void Wayang::JatayuTalkBob()
{
    moveWhatServoWithBitBang(2, 160, 350);
    moveWhatServoWithBitBang(2, 180, 350);
}

// Takes 700 ms
void Wayang::JatayuTalkSide()
{
    moveWhatServoWithBitBang(1, 25, 350);
    moveWhatServoWithBitBang(1, 65, 350);
}

// Takes 700 ms
void Wayang::JatayuDefaultFly()
{
    // defaultFaceOrientation();
    moveWhatServoWithBitBang(1, 45, 200);
    moveWhatServoWithBitBang(2, 0, 500);
}

// parse values from outside class into class for direct control of servos
void Wayang::JatayuDirectControl(uint8_t ServoNum, uint8_t Deg, int Duration)
{
    moveWhatServoWithBitBang(ServoNum, Deg, Duration);
}

// Takes 700 ms
void Wayang::SitaPointToFront()
{
    moveWhatServoWithBitBang(1, 180, 500);
    moveWhatServoWithBitBang(2, 150, 200);
}

// Takes 700 ms
void Wayang::SitaLowPointToFront()
{
    moveWhatServoWithBitBang(1, 90, 500);
    moveWhatServoWithBitBang(2, 130, 200);
}

// Takes 700 ms
void Wayang::SitaMiddleFront()
{
    moveWhatServoWithBitBang(1, 150, 500);
    moveWhatServoWithBitBang(2, 120, 200);
}

// Takes 700 ms
void Wayang::SitaDownFront()
{
    moveWhatServoWithBitBang(1, 0, 500);
    moveWhatServoWithBitBang(2, 121, 200);
}

// Takes 900 ms
void Wayang::SitaPointToSelf()
{
    moveWhatServoWithBitBang(2, 60, 200);
    moveWhatServoWithBitBang(1, 180, 500);
    moveWhatServoWithBitBang(2, 100, 200);
}

// Takes 700 ms
void Wayang::RahwanaPointToFront()
{
    moveWhatServoWithBitBang(3, 0, 500);
    moveWhatServoWithBitBang(4, 72, 200);
}

// Takes 700 ms
void Wayang::RahwanaLowPointToFront()
{
    moveWhatServoWithBitBang(3, 90, 500);
    moveWhatServoWithBitBang(4, 80, 200);
}

// Takes 500 ms needs to Down Front first
void Wayang::RahwanaMiddleFront()
{
    moveWhatServoWithBitBang(3, 0, 500);
}

// Takes 700 ms
void Wayang::RahwanaDownFront()
{
    moveWhatServoWithBitBang(3, 180, 500);
    moveWhatServoWithBitBang(4, 87, 200);
}

// Takes 900 ms
void Wayang::RahwanaPointToSelf()
{
    moveWhatServoWithBitBang(4, 140, 200);
    moveWhatServoWithBitBang(3, 0, 500);
    moveWhatServoWithBitBang(4, 110, 200);
}

void Wayang::defaultHandPosition()
{
    // check if this RahwanaSita
    if (getServoPin5() != 0)
    {
        RahwanaDownFront();
        SitaDownFront();
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
        moveWhatServoWithBitBang(5, 0, 1200);
    }
    else
    {
        Serial2.println(F("This is Jatayu"));
        moveWhatServoWithBitBang(3, 180, 1200);
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
        moveWhatServoWithBitBang(5, 180, 1200);
    }
    else
    {
        Serial2.println(F("This is Jatayu"));
        moveWhatServoWithBitBang(3, 0, 1200);
    }
}