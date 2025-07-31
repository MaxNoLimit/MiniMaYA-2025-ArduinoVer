#include "Wayang.hpp"
#include "main.hpp"
#include <stdint.h>
#include <HardwareTimer.h>

WayangHandServo::WayangHandServo(uint8_t leftOrRight)
{
    switch (leftOrRight)
    {
    case whatSideServo::LEFT:
        setServoPin1(SERVO1_1);
        setServoPin2(SERVO2_1);
        setServoPin3(SERVO3_1);
        setServoPin4(SERVO4_1);
        setServoPin5(SERVO5_1);
        break;

    case whatSideServo::RIGHT:
        setServoPin1(SERVO1_2);
        setServoPin2(SERVO2_2);
        setServoPin3(SERVO3_2);
        setServoPin4(0);
        setServoPin5(0);
        break;
    }
}

WayangHandServo::~WayangHandServo()
{
    // do nothing
}

void WayangHandServo::defaultPosition()
{
    moveWhatServo(1, 180, 500);
    moveWhatServo(2, 90, 200);
    moveWhatServo(3, 0, 500);
    moveWhatServo(4, 90, 200);
}

void WayangHandServo::resetArray()
{
    for (int i = 0; i < 4; i++)
    {
        if ((i + 1) % 2 == 0)
        {
            setCurrentDegServo(i + 1, 90);
        }

        setCurrentDegServo(i + 1, 0);
    }
}

uint8_t WayangHandServo::getCurrentDegServo(uint8_t servoNum)
{
    uint8_t result;
    switch (servoNum)
    {
    case 1:
        result = this->currentDeg[0];
        break;

    case 2:
        result = this->currentDeg[1];
        break;

    case 3:
        result = this->currentDeg[2];
        break;

    case 4:
        result = this->currentDeg[3];
        break;
    }
    return result;
}

void WayangHandServo::setCurrentDegServo(uint8_t servoNum, uint8_t degree)
{
    switch (servoNum)
    {
    case 1:
        this->currentDeg[0] = degree;
        break;

    case 2:
        this->currentDeg[1] = degree;
        break;

    case 3:
        this->currentDeg[2] = degree;
        break;

    case 4:
        this->currentDeg[3] = degree;
        break;
    }
}

void WayangHandServo::moveWhatServo(uint8_t servoNum, uint8_t degree, int desiredDuration)
{
    uint32_t selectedPin;
    switch (servoNum)
    {
    case 1:
        selectedPin = getServoPin1();
        break;

    case 2:
        selectedPin = getServoPin2();
        break;

    case 3:
        selectedPin = getServoPin3();
        break;

    case 4:
        selectedPin = getServoPin4();
        break;
    case 5:
        selectedPin = getServoPin5();
        break;
    }

    if (selectedPin == 0)
    {
        Serial2.println(F("ignoring this since the pin is no available\n"));
        return;
    }

    Serial2.println("Servo pin of " + String(selectedPin) + " is now moving");
    uint8_t curdeg = getCurrentDegServo(servoNum);
    if (curdeg - degree != 0)
    {
        int waveAmount = desiredDuration / 20;
        delay(desiredDuration % 20);
        int degmismatch = abs(curdeg - degree);
        int largemismatch = degmismatch;
        int divVar = 1;

        // subdivide until nondec
        while (waveAmount / largemismatch < 1)
        {
            largemismatch = largemismatch / 2;
            divVar = divVar * 2;
        }

        int mismatchremainder = degmismatch % divVar;
        int remainder = waveAmount % largemismatch;

        if (curdeg > degree)
        {
            if (mismatchremainder != 0)
            {
                for (int i = 0; i < remainder; i++)
                {
                    digitalWrite(selectedPin, HIGH);
                    delayMicroseconds(degreeToDelay(curdeg - mismatchremainder));
                    digitalWrite(selectedPin, LOW);
                    delayMicroseconds(getWavePeriod() - degreeToDelay(curdeg - mismatchremainder)); // to deal with 16ms delayMicroseconds bug
                }
            }

            for (int i = 0; i < largemismatch; i++)
            {
                for (int j = 0; j < waveAmount / largemismatch; j++)
                {
                    digitalWrite(selectedPin, HIGH);
                    delayMicroseconds(degreeToDelay(curdeg - i * divVar));
                    digitalWrite(selectedPin, LOW);
                    delayMicroseconds(getWavePeriod() - degreeToDelay(curdeg - i * divVar));
                }
            }

            if (mismatchremainder == 0 && remainder != 0)
            {
                for (int i = 0; i < remainder; i++)
                {
                    digitalWrite(selectedPin, HIGH);
                    delayMicroseconds(degreeToDelay(degree));
                    digitalWrite(selectedPin, LOW);
                    delayMicroseconds(getWavePeriod() - degreeToDelay(degree));
                }
            }
        }
        else
        {
            if (mismatchremainder != 0)
            {
                for (int i = 0; i < remainder; i++)
                {
                    digitalWrite(selectedPin, HIGH);
                    delayMicroseconds(degreeToDelay(curdeg + mismatchremainder));
                    digitalWrite(selectedPin, LOW);
                    delayMicroseconds(getWavePeriod() - degreeToDelay(curdeg + mismatchremainder));
                }
            }

            for (int i = 0; i < largemismatch; i++)
            {
                for (int j = 0; j < waveAmount / largemismatch; j++)
                {
                    digitalWrite(selectedPin, HIGH);
                    delayMicroseconds(degreeToDelay(curdeg + i * divVar));
                    digitalWrite(selectedPin, LOW);
                    delayMicroseconds(getWavePeriod() - degreeToDelay(curdeg + i * divVar));
                }
            }
            // int hajime, sutopu = 0;

            if (mismatchremainder == 0 && remainder != 0)
            {
                for (int i = 0; i < remainder; i++)
                {
                    // hajime = 0;
                    // sutopu = 0;
                    // hajime = millis();
                    digitalWrite(selectedPin, HIGH);
                    delayMicroseconds(degreeToDelay(degree));
                    digitalWrite(selectedPin, LOW);
                    delayMicroseconds(getWavePeriod() - degreeToDelay(degree));

                    // sutopu = millis();
                    // Serial.println("Duration taken: " + (String)(sutopu - hajime) + "\n");
                }
            }
        }

        setCurrentDegServo(servoNum, degree);
    }
    else
    {
        // do nothing
        delay(desiredDuration);
    }
    Serial2.println("Servo pin of " + String(selectedPin) + " is done moving!!");
}

void WayangHandServo::moveWhatServoWithTimer(uint8_t servoNumber, uint8_t degree, int desiredDuration)
{
    HardwareTimer *ServoTimer = NULL;
    uint32_t selectedPin;
    switch (servoNumber)
    {
    case 1:
        selectedPin = getServoPin1();
        break;

    case 2:
        selectedPin = getServoPin2();
        break;

    case 3:
        selectedPin = getServoPin3();
        break;

    case 4:
        selectedPin = getServoPin4();
        break;
    case 5:
        selectedPin = getServoPin5();
        break;
    }

    if (selectedPin == 0)
    {
        Serial2.println(F("ignoring this since the pin is no available\n"));
        return;
    }
    TIM_TypeDef *instance = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(selectedPin), PinMap_PWM);
    if (instance == NULL)
    {
        Serial2.println("Error: Pin is not PWM capable or timer instance not found!");
        return;
    }
    ServoTimer = new HardwareTimer(instance);

    ServoTimer->setPWM(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(selectedPin), PinMap_PWM)), selectedPin, 50, 0); // Start with 0ms pulse
                                                                                                                            // Set the compare format to microseconds

    uint8_t degDiff = abs(currentDeg[servoNumber - 1] - degree);
    if (degDiff != 0)
    {
        // uint8_t waveAmount = duration / 20;
        delay(desiredDuration % 20);
        uint8_t largemismatch = degDiff; // must
        uint8_t divVar = 1;              // must

        // subdivide until nondec
        while ((desiredDuration / 20) / largemismatch < 1)
        {
            largemismatch = largemismatch / 2;
            divVar = divVar * 2;
        }

        int sign = 1;
        if (currentDeg[servoNumber - 1] > degree)
        {
            sign = sign * (-1);
        }

        if ((degDiff % divVar) != 0)
        {
            for (int i = 0; i < ((desiredDuration / 20) % largemismatch); i++)
            {
                ServoTimer->setCaptureCompare(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(selectedPin), PinMap_PWM)), degreeToDelay(currentDeg[servoNumber - 1] + (degDiff % divVar) * sign), MICROSEC_COMPARE_FORMAT);
                delay(20);
            }
        }

        for (int i = 0; i < largemismatch; i++)
        {
            for (int j = 0; j < ((desiredDuration / 20) / largemismatch); j++)
            {
                ServoTimer->setCaptureCompare(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(selectedPin), PinMap_PWM)), degreeToDelay(currentDeg[servoNumber - 1] + (i * divVar) * sign), MICROSEC_COMPARE_FORMAT);
                delay(20);
            }
        }

        if ((degDiff % divVar) == 0 && ((desiredDuration / 20) % largemismatch) != 0)
        {
            for (int i = 0; i < ((desiredDuration / 20) % largemismatch); i++)
            {
                ServoTimer->setCaptureCompare(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(selectedPin), PinMap_PWM)), degreeToDelay(degree), MICROSEC_COMPARE_FORMAT);
                delay(20);
            }
        }
    }
    else
    {
        ServoTimer->setCaptureCompare(STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(selectedPin), PinMap_PWM)), degreeToDelay(degree), MICROSEC_COMPARE_FORMAT);
        delay(desiredDuration);
    }

    currentDeg[servoNumber - 1] = degree;
}

void WayangHandServo::moveWhatServoFlick(uint8_t servoNumber, uint8_t degree, int desiredDuration)
{
    uint32_t selectedPin;
    switch (servoNumber)
    {
    case 1:
        selectedPin = getServoPin1();
        break;

    case 2:
        selectedPin = getServoPin2();
        break;

    case 3:
        selectedPin = getServoPin3();
        break;

    case 4:
        selectedPin = getServoPin4();
        break;
    case 5:
        selectedPin = getServoPin5();
        break;
    }
    if (selectedPin == 0)
    {
        Serial2.println(F("ignoring this since the pin is no available\n"));
        return;
    }
    delay(desiredDuration % 20);
    for (int i = 0; i < desiredDuration / 20; i++)
    {
        digitalWrite(selectedPin, HIGH);
        delayMicroseconds(degreeToDelay(degree));
        digitalWrite(selectedPin, LOW);
        delayMicroseconds(getWavePeriod() - degreeToDelay(degree));
    }
    setCurrentDegServo(servoNumber, degree);
}