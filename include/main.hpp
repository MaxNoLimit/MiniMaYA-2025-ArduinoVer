#ifndef INC_MAIN_HPP
#define INC_MAIN_HPP

#include <Arduino.h>

// TMC2209 ... (1)
#define EN_PIN PB3
#define STEP_PIN PB4
#define DIR_PIN PB5
#define DIAG_PIN PB6

// TMC2209 ... (2)
#define EN2_PIN PA15
#define STEP2_PIN PA8
#define DIR2_PIN PA7
#define DIAG2_PIN PB7

// Rahwana's and Sita's Servos
#define SERVO1_1 PB0
#define SERVO2_1 PB1
#define SERVO3_1 PB2
#define SERVO4_1 PB10
#define SERVO5_1 PB12

// Jatayu's Servos
#define SERVO1_2 PB15
#define SERVO2_2 PB14
#define SERVO3_2 PB13

namespace MainFunction
{
    void System_Setup();
    void TMC_DiagHandler_Jatayu();
    void TMC_DiagHandler_RahwanaSita();
    void USART_Comm();
}

void GPIO_Init();

extern bool isJatayuStalled;
extern bool isRahwanaSitaStalled;
#endif // INCH_MAIN_HPP