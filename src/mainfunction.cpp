#include "main.hpp"
#include "TMC2209.hpp"
#include "config.hpp"
#include "SoundSystem.hpp"
#include <STM32FreeRTOS.h>
#include "Wayang.hpp"

TMCWayang Jatayu_Horizontal("right");
Wayang Jatayu(whatSideServo::RIGHT);
TMCWayang RahwanaSita_Horizontal("left");
Wayang RahwanaSita(whatSideServo::LEFT);

bool isJatayuStalled = false;
bool isRahwanaSitaStalled = false;

TaskHandle_t USARTCommTask_Handler, PlayTask_Handler;

void MainFunction::System_Setup()
{
    /* Sound System */
    SoundSystem::Init();

    /* Wayang TMC */
    Jatayu_Horizontal.Init();      // Initialize the TMC2209 driver for Jatayu
    RahwanaSita_Horizontal.Init(); // Initialize the TMC2209 driver for Rahwana and Sita

    Serial2.println(F("System setup all done!!"));

    /* Create USART Comm Handler*/
    BaseType_t usart_task_result =
        xTaskCreate(
            USART_Comm_Task,
            "USART_Comm_Task",
            USART_COMM_TASK_HEAP,
            NULL,
            1,
            &USARTCommTask_Handler);

    /* Check the result */
    if (usart_task_result == pdPASS)
    {
        Serial2.println(F("USART Task created!!"));
    }
    else
    {
        Serial2.println(F("Failed to create USART task!"));
    }

    vTaskStartScheduler();
}

static void USART_Comm_Task(void *pvParam)
{
    UNUSED(pvParam);
    Serial2.println(F("Hi, I'm Mini MaYA 2025 USART Handler"));
    for (;;)
    {
        String command = Serial2.readStringUntil('\n');
        if (command == "PlayTheShow")
        {
            Serial2.println(F("PlayTheShow Task created!!"));
            /* Create play task */
            xTaskCreate(
                Play_Task,
                "Play_Task",
                PLAY_TASK_HEAP,
                NULL,
                1,
                &PlayTask_Handler);
        }
        else if (command == "PauseTheShow")
        {
            Serial2.println(F("The show is paused!!"));
            /* Pause the play task */
            if (PlayTask_Handler != NULL)
            {
                vTaskSuspend(PlayTask_Handler);
            }
        }
        else if (command == "AbortTheShow")
        {
            Serial2.println(F("The show is aborted!!"));
            /* Kill the play task */
            if (PlayTask_Handler != NULL)
            {
                vTaskDelete(PlayTask_Handler);
            }
        }

        else if (command == "VSlotCalibration")
        {
            Serial2.println(F("VSlotCalibration Task created!!"));
            /* Create vslotcalibration task */
            MainFunction::Calibration::VSlotCalibration();
            Serial2.write("VSDone");
        }
        else if (command == "WayangServo")
        {
            Serial2.println(F("WayangServo Task created!!"));
            /* Create wayangservo task */
            MainFunction::Calibration::Wayang_Servo();
            Serial2.write("WSDone");
        }
        else if (command == "move1")
        {
            Serial2.println(F("move1 command"));
            Jatayu_Horizontal.WalkToScene();
        }
        else if (command == "homing1")
        {
            Serial2.println(F("homing1 command"));
            Jatayu_Horizontal.DefaultPosition();
        }
        else if (command == "move2")
        {
            Serial2.println(F("move2 command"));
            RahwanaSita_Horizontal.WalkToScene();
        }
        else if (command == "homing2")
        {
            Serial2.println(F("homing2 command"));
            RahwanaSita_Horizontal.DefaultPosition();
        }
    }
}

static void Play_Task(void *pvParam)
{
    UNUSED(pvParam);
    Serial2.println(F("Playing the show!!"));
    for (;;)
    {

        /* for killing its own task */
        Serial2.println(F("Task done!!"));
        vTaskDelete(NULL);
    }
}

/* Play Feature */
void MainFunction::Play::Play_The_Show()
{
}

void MainFunction::Play::Pause_The_Show()
{
}

void MainFunction::Play::Abort_The_Show()
{
}

/* Calibration Feature */
void MainFunction::Calibration::VSlotCalibration()
{
    /* Homing both wayang */
    Jatayu_Horizontal.DefaultPosition();
    RahwanaSita_Horizontal.DefaultPosition();

    /* Measure each for calibration */
    Jatayu_Horizontal.MeasureMovement();
    RahwanaSita_Horizontal.MeasureMovement();

    /* Homing for default */
    Jatayu_Horizontal.DefaultPosition();
    RahwanaSita_Horizontal.DefaultPosition();
}

void MainFunction::Calibration::Wayang_Servo()
{
    Jatayu_Horizontal.LeaveTheScene();
    Jatayu.flick();
    Jatayu.defaultFaceOrientation();

    RahwanaSita_Horizontal.LeaveTheScene();
    // RahwanaSita.flick();
    // RahwanaSita.defaultFaceOrientation();
    RahwanaSita.defaultHandPosition();
}
