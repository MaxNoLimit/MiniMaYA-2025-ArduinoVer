#include "TMC2209.hpp"
#include "main.hpp"
#include <TMCStepper.h>
#include "config.hpp"

HardwareSerial Serial1(PA10, PA9);

TMC2209Stepper TMCDriver_Right(&MY_TMC_USART_PORT, R_SENSE, DRIVER_ADDRESS1);
TMC2209Stepper TMCDriver_Left(&MY_TMC_USART_PORT, R_SENSE, DRIVER_ADDRESS2);

TMCWayang::TMCWayang(String leftOrRight)
{
    if (leftOrRight == "right")
    {
        setDriverAddress(DRIVER_ADDRESS1);
        setEnablePin(EN_PIN);
        setStepPin(STEP_PIN);
        setDirPin(DIR_PIN);
        setDiagPin(DIAG_PIN);
        setWalkToSceneDir(0);
        setLeaveTheSceneDir(1);
    }
    else if (leftOrRight == "left")
    {
        setDriverAddress(DRIVER_ADDRESS2);
        setEnablePin(EN2_PIN);
        setStepPin(STEP2_PIN);
        setDirPin(DIR2_PIN);
        setDiagPin(DIAG2_PIN);
        setWalkToSceneDir(1);
        setLeaveTheSceneDir(0);
    }
}

TMCWayang::~TMCWayang()
{
    // Destructor logic if needed
}

void TMCWayang::Init()
{
    MY_TMC_USART_PORT.begin(115200);
    digitalWrite(getEnablePin(), LOW);

    switch (getDriverAddress())
    {
    case 0b00:
        TMCDriver_Right.beginSerial(115200);

        TMCDriver_Right.begin();
        TMCDriver_Right.toff(4);
        TMCDriver_Right.blank_time(24);
        TMCDriver_Right.rms_current(1000); // mA
        TMCDriver_Right.microsteps(4);
        TMCDriver_Right.TCOOLTHRS(0xFFFFF); // 20bit max
        TMCDriver_Right.semin(5);

        TMCDriver_Right.semax(2);
        TMCDriver_Right.sedn(0b01);
        TMCDriver_Right.SGTHRS(100);
        break;

    case 0b01:
        TMCDriver_Left.beginSerial(115200);

        TMCDriver_Left.begin();
        TMCDriver_Left.toff(4);
        TMCDriver_Left.blank_time(24);
        TMCDriver_Left.rms_current(1000); // mA
        TMCDriver_Left.microsteps(4);
        TMCDriver_Left.TCOOLTHRS(0xFFFFF); // 20bit max
        TMCDriver_Left.semin(5);

        TMCDriver_Left.semax(2);
        TMCDriver_Left.sedn(0b01);
        TMCDriver_Left.SGTHRS(100);
        break;
    }
    digitalWrite(getEnablePin(), HIGH);
}

void TMCWayang::Spin_Steps(int steps, uint8_t dir)
{
    // enable driver to spin
    Serial2.println("Attempting to spin driver " + String(getDriverAddress() + 1));
    switch (getDriverAddress())
    {
    case 0b00:
        TMCDriver_Right.TCOOLTHRS(0xFFFFF);
        TMCDriver_Right.SGTHRS(100);
        TMCDriver_Right.microsteps(4);
        break;

    case 0b01:
        TMCDriver_Left.TCOOLTHRS(0xFFFFF);
        TMCDriver_Left.SGTHRS(100);
        TMCDriver_Left.microsteps(4);
        break;
    }

    digitalWrite(getEnablePin(), LOW);
    digitalWrite(getDirPin(), dir);

    for (int i = 0; i < steps; i++)
    {
        digitalWrite(getStepPin(), HIGH);
        delayMicroseconds(FREQ_NORMALSPIN);
        digitalWrite(getStepPin(), LOW);
        delayMicroseconds(FREQ_NORMALSPIN);
        // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    }

    // disable driver to spin
    digitalWrite(getEnablePin(), HIGH);
    Serial2.println("Done Spinning TMC: " + String(getDriverAddress() + 1));
}

void TMCWayang::WalkToScene()
{
    setCurrentDir(getWalkToSceneDir());
    Spin_Steps(mm_distance_to_steps((float)(300.0 - DELRIN_SPACER_DISTANCE)), getWalkToSceneDir());
}

void TMCWayang::LeaveTheScene()
{
    DefaultPosition();
}

void TMCWayang::DefaultPosition()
{
    setStalledStatus(false);
    Serial2.println("Homing action driver " + String(getDriverAddress() + 1) + "...");
    switch (getDriverAddress())
    {
    case 0b00:
        TMCDriver_Right.TCOOLTHRS(0x7FFFF);
        TMCDriver_Right.SGTHRS(HOMING_TMC_THRS_1);
        TMCDriver_Right.microsteps(4);
        break;

    case 0b01:
        TMCDriver_Left.TCOOLTHRS(0x7FFFF);
        TMCDriver_Left.SGTHRS(HOMING_TMC_THRS_2);
        TMCDriver_Left.microsteps(4);
        break;
    }

    // enable driver to spin
    digitalWrite(getEnablePin(), LOW);
    digitalWrite(getDirPin(), getLeaveTheSceneDir()); // Set direction to leave the scene
    setCurrentDir(getLeaveTheSceneDir());
    // Spin function while there is no interrupt
    while (getStalledStatus() != true)
    {
        if (digitalRead(getDiagPin()) == HIGH)
        {
            break;
        }
        digitalWrite(getStepPin(), HIGH);
        delayMicroseconds(1200);
        digitalWrite(getStepPin(), LOW);
        delayMicroseconds(1200);
    }
    // disable driver to spin
    digitalWrite(getEnablePin(), HIGH);
    setStalledStatus(!getStalledStatus());
    Serial2.println("Homing action " + String(getDriverAddress() + 1) + " done!!");
}

void TMCWayang::MeasureMovement()
{
    setStalledStatus(false);
    Serial2.println(F("Start meassuring!"));
    int nCount = 0;
    switch (getDriverAddress())
    {
    case 0b00:
        TMCDriver_Right.TCOOLTHRS(0x7FFFF);
        TMCDriver_Right.SGTHRS(HOMING_STALL_VALUE);
        TMCDriver_Right.microsteps(4);
        break;

    case 0b01:
        TMCDriver_Left.TCOOLTHRS(0x7FFFF);
        TMCDriver_Left.SGTHRS(HOMING_STALL_VALUE);
        TMCDriver_Left.microsteps(4);
        break;
    }

    // enable driver to spin
    digitalWrite(getEnablePin(), LOW);
    digitalWrite(getDirPin(), getWalkToSceneDir()); // Set direction to walk to the scene

    while (getStalledStatus() != true)
    {
        if (digitalRead(getDiagPin()) == HIGH)
        {
            break;
        }
        digitalWrite(getStepPin(), HIGH);
        delayMicroseconds(1200);
        digitalWrite(getStepPin(), LOW);
        delayMicroseconds(1200);
        nCount++;
        // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    }

    // disable driver to spin
    digitalWrite(getEnablePin(), HIGH);
    setStalledStatus(!getStalledStatus()); // Reset the stall status after measuring
    Serial2.println(F("Measure action done!!"));
    setConstant((float)nCount / (float)RAIL_DISTANCE); // Calculate the constant value based on the number of steps and rail distance
    Serial2.println("Constant value" + String(getDriverAddress() + 1) + ": " + (String)getConstant());
}

void TMCWayang::DiagHandler()
{

    setStalledStatus(!getStalledStatus()); // Toggle the stall status
    Serial2.println("Stall detected on driver " + String(getDriverAddress() + 1) + ": " + String(getStalledStatus()));
    Spin_Steps(1, !getCurrentDir()); // Spin one step in the opposite direction to clear the stall
}

int TMCWayang::mm_distance_to_steps(float mm_distance)
{
    // Convert mm distance to steps based on the constant value
    if (getConstant() == 0.0f)
    {
        Serial2.println("Constant value is not set, returning 0 steps.");
        return 0;
    }
    int steps = (int)(mm_distance * getConstant());
    Serial2.println("Converted " + String(mm_distance) + " mm to " + String(steps) + " steps.");
    return steps;
}