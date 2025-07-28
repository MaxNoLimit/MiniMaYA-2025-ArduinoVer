#ifndef INC_TMC2209_HPP
#define INC_TMC2209_HPP

#include <stdint.h>
#include <Arduino.h>

#define HOMING_STALL_VALUE 30
#define DRIVER_ADDRESS1 0b00 // TMC2209 Driver address according to MS1 and MS2
#define DRIVER_ADDRESS2 0b01
#define MY_TMC_USART_PORT Serial1

#define R_SENSE 0.11f // Match to your driver
                      // SilentStepStick series use 0.11
                      // UltiMachine Einsy and Archim2 boards use 0.2
                      // Panucatt BSD2660 uses 0.1
                      // Watterott TMC5160 uses 0.075

class TMCWayang
{
private:
    uint8_t driverAddress;
    uint8_t enablePin;
    uint8_t stepPin;
    uint8_t dirPin;
    uint8_t diagPin;
    uint8_t walkToSceneDir;
    uint8_t leaveTheSceneDir;
    uint8_t currentDir;

    bool isStallDetected = false;    // Flag to indicate if a stall is detected
    float constantMoveValue = 6.74f; // Default value, can be set later
public:
    TMCWayang(String leftOrRight);
    ~TMCWayang();

    /* Setter */
    void setDriverAddress(uint8_t address) { this->driverAddress = address; }
    void setEnablePin(uint8_t pin) { this->enablePin = pin; }
    void setStepPin(uint8_t pin) { this->stepPin = pin; }
    void setDirPin(uint8_t pin) { this->dirPin = pin; }
    void setDiagPin(uint8_t pin) { this->diagPin = pin; }
    void setWalkToSceneDir(uint8_t dir) { this->walkToSceneDir = dir; }
    void setLeaveTheSceneDir(uint8_t dir) { this->leaveTheSceneDir = dir; }
    void setStalledStatus(bool status) { this->isStallDetected = status; }
    void setCurrentDir(uint8_t dir) { this->currentDir = dir; }

    /* Getter */
    const uint8_t getDriverAddress() { return this->driverAddress; }
    const uint8_t getEnablePin() { return this->enablePin; }
    const uint8_t getStepPin() { return this->stepPin; }
    const uint8_t getDirPin() { return this->dirPin; }
    const uint8_t getDiagPin() { return this->diagPin; }
    const uint8_t getWalkToSceneDir() { return this->walkToSceneDir; }
    const uint8_t getLeaveTheSceneDir() { return this->leaveTheSceneDir; }
    const bool getStalledStatus() { return this->isStallDetected; }
    const uint8_t getCurrentDir() { return this->currentDir; }

    void Init();
    void Spin_Steps(int steps, uint8_t dir);
    void WalkToScene();
    void LeaveTheScene();

    void DefaultPosition();
    void MeasureMovement();
    void InterruptMiddleware();
    void DiagHandler();

    void setConstant(float value) { this->constantMoveValue = value; }
    float getConstant() { return this->constantMoveValue; }
    int mm_distance_to_steps(float mm_distance);
};

#endif // INC_TMC2209_HPP