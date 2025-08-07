#ifndef INC_WAYANG_HPP
#define INC_WAYANG_HPP

#include <Arduino.h>
#include <stdint.h>

enum whatSideServo
{
    RIGHT = 0,
    LEFT = 1
};

class WayangHandServo
{
private:
    const int wavePeriod = 20000; // in microseconds
    const int amountOfWaves = 100;
    uint32_t servoPin1;
    uint32_t servoPin2;
    uint32_t servoPin3;
    uint32_t servoPin4;
    uint32_t servoPin5;
    uint8_t currentDeg[4] = {0, 91, 0, 91};

public:
    WayangHandServo(uint8_t leftOrRight);
    ~WayangHandServo();
    void defaultPosition();
    void moveWhatServo(uint8_t servoNumber, uint8_t degree, int desiredDuration);
    void moveWhatServoWithBitBang(uint8_t servoNumber, uint8_t degree, int desiredDuration);
    void moveWhatServoFlick(uint8_t servoNumber, uint8_t degree, int desiredDuration);

    /* Simple one for servo move */
    // void flickWhatServo(uint8_t servoNumber, uint8_t degree, int desiredDuration);

    void setServoPin1(uint32_t pin) { this->servoPin1 = pin; }
    void setServoPin2(uint32_t pin) { this->servoPin2 = pin; }
    void setServoPin3(uint32_t pin) { this->servoPin3 = pin; }
    void setServoPin4(uint32_t pin) { this->servoPin4 = pin; }
    void setServoPin5(uint32_t pin) { this->servoPin5 = pin; }

    uint32_t getServoPin1() { return this->servoPin1; }
    uint32_t getServoPin2() { return this->servoPin2; }
    uint32_t getServoPin3() { return this->servoPin3; }
    uint32_t getServoPin4() { return this->servoPin4; }
    uint32_t getServoPin5() { return this->servoPin5; }

    int getAmountOfWaves() { return this->amountOfWaves; }
    int getWavePeriod() { return this->wavePeriod; }
    uint32_t degreeToDelay(uint8_t degree) { return map(degree, 0, 180, 500, 2500); }

    void setCurrentDegServo(uint8_t servoNum, uint8_t degree);
    uint8_t getCurrentDegServo(uint8_t servoNum);

    void resetArray();
};

class Wayang : WayangHandServo
{
private:
public:
    Wayang(uint16_t leftOrRight) : WayangHandServo(leftOrRight) {} // Default constructor
    ~Wayang() {}                                                   // Destructor
    void RahwanaPointToFront();
    void RahwanaLowPointToFront();
    void RahwanaMiddleFront();
    void RahwanaDownFront();
    void RahwanaPointToSelf();

    void SitaPointToFront();
    void SitaLowPointToFront();
    void SitaMiddleFront();
    void SitaDownFront();
    void SitaPointToSelf();

    void JatayuTakeoff();
    void JatayuDive();
    void JatayuTalon();
    void JatayuDefaultFly();
    void JatayuAttack();
    void JatayuTalkBob();
    void JatayuTalkSide();
    void JatayuDirectControl(uint8_t ServoNum, uint8_t Deg, int Duration);

    void defaultHandPosition();
    void flick();
    void defaultFaceOrientation();
};

#endif // INC_WAYANG_HPP