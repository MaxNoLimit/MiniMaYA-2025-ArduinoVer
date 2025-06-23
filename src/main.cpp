#include "main.hpp"

void setup()
{
  Serial2.begin(115200);
  GPIO_Init();
  Serial2.println(F("Mini MaYA 2025 Serial Monitor"));
  MainFunction::System_Setup();
}

void loop()
{
  // MainFunction::USART_Comm();
}

void GPIO_Init()
{
  /* TMC2209 */
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH);
  pinMode(EN2_PIN, OUTPUT);
  digitalWrite(EN2_PIN, HIGH);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(DIAG_PIN, INPUT);

  pinMode(STEP2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(DIAG2_PIN, INPUT);

  /* SERVOS */
  pinMode(SERVO1_1, OUTPUT);
  pinMode(SERVO2_1, OUTPUT);
  pinMode(SERVO3_1, OUTPUT);
  pinMode(SERVO4_1, OUTPUT);
  pinMode(SERVO5_1, OUTPUT);

  pinMode(SERVO1_2, OUTPUT);
  pinMode(SERVO2_2, OUTPUT);
  pinMode(SERVO3_2, OUTPUT);
}