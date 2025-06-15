#include "main.hpp"
#include "TMC2209.hpp"
#include "config.hpp"
#include "SoundSystem.hpp"
#include <STM32FreeRTOS.h>

TMCWayang Jatayu("right");
TMCWayang RahwanaSita("left");

bool isJatayuStalled = false;
bool isRahwanaSitaStalled = false;

TaskHandle_t USARTCommTask_Handler, PlayTask_Handler;

void MainFunction::System_Setup()
{
    /* Sound System */
    SoundSystem::Init();

    /* Wayang TMC */
    Jatayu.Init();      // Initialize the TMC2209 driver for Jatayu
    RahwanaSita.Init(); // Initialize the TMC2209 driver for Rahwana and Sita

    /* Checking if Serial2 is connected */
    if (Serial2.available())
    {
        /* Create USART Comm Handler*/
        xTaskCreate(
            USART_Comm_Task,
            "USART_Comm_Task",
            USART_COMM_TASK_HEAP,
            NULL,
            2,
            &USARTCommTask_Handler);
    }
}

static void USART_Comm_Task(void *pvParam)
{
    for (;;)
    {
        String command = Serial2.readStringUntil('\n');
        if (command == "PlayTheShow")
        {
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
            /* Pause the play task */
            vTaskSuspend(PlayTask_Handler);
        }
        else if (command == "AbortTheShow")
        {
            /* Kill the play task */
            vTaskDelete(PlayTask_Handler);
        }

        else if (command == "VSlotCalibration")
        {
            /* Create vslotcalibration task */
            xTaskCreate(
                VSlotCalibration_Task,
                "VSlotCalibration_Task",
                VSLOT_CALIBRATION_TASK_HEAP,
                NULL,
                1,
                NULL);
        }
        else if (command == "WayangServo")
        {
            /* Create wayangservo task */
            xTaskCreate(
                WayangServoCalibration_Task,
                "WayangServoCalibration_Task",
                WAYANGSERVO_TASK_HEAP,
                NULL,
                1,
                NULL);
        }
    }
}

static void Play_Task(void *pvParam)
{
    for (;;)
    {

        /* for killing its own task */
        vTaskDelete(NULL);
    }
}
static void VSlotCalibration_Task(void *pvParam)
{
    for (;;)
    {
        /* Do VSlotCalibration feature */
        MainFunction::Calibration::VSlotCalibration();
        /* for killing its own task */
        vTaskDelete(NULL);
    }
}
static void WayangServoCalibration_Task(void *pvParam)
{
    for (;;)
    {
        /* Do WayangServoCalibration feature */
        MainFunction::Calibration::Wayang_Servo();
        /* for killing its own task */
        vTaskDelete(NULL);
    }
}

void MainFunction::USART_Comm()
{
    // This function can be used to handle USART communication
    // For example, reading commands from Serial2 and processing them
    if (Serial2.available())
    {
        String command = Serial2.readStringUntil('\n'); // Read command from Serial2 until newline
        if (command == "VSlotCalibration")
        {
            Jatayu.DefaultPosition();      // Call the default position method for Jatayu
            RahwanaSita.DefaultPosition(); // Call the default position method for Rahwana and Sita

            Jatayu.MeasureMovement();                                                                                     // Measure movement for Jatayu
            Jatayu.Spin_Steps(Jatayu.mm_distance_to_steps(100.0 - DELRIN_SPACER_DISTANCE), Jatayu.getLeaveTheSceneDir()); // Spin Jatayu for 100mm distance
            RahwanaSita.MeasureMovement();
            RahwanaSita.Spin_Steps(RahwanaSita.mm_distance_to_steps(100.0 - DELRIN_SPACER_DISTANCE), RahwanaSita.getLeaveTheSceneDir()); // Spin Rahwana and Sita for 100mm distance
            Serial2.println(F("V-Slot Calibration Done!"));
        }
        else if (command == "move1")
        {
            Jatayu.WalkToScene();
        }
        else if (command == "move2")
        {
            RahwanaSita.WalkToScene();
        }
        else if (command == "homing1")
        {
            Jatayu.DefaultPosition();
        }
        else if (command == "homing2")
        {
            RahwanaSita.DefaultPosition();
        }
        else if (command == "measure1")
        {
            Jatayu.MeasureMovement();                                                                                     // Measure movement for Jatayu
            Jatayu.Spin_Steps(Jatayu.mm_distance_to_steps(100.0 - DELRIN_SPACER_DISTANCE), Jatayu.getLeaveTheSceneDir()); // Spin Jatayu for 100mm distance
        }
        else if (command == "measure2")
        {
            RahwanaSita.MeasureMovement();                                                                                               // Measure movement for Rahwana and Sita
            RahwanaSita.Spin_Steps(RahwanaSita.mm_distance_to_steps(100.0 - DELRIN_SPACER_DISTANCE), RahwanaSita.getLeaveTheSceneDir()); // Spin Rahwana and Sita for 100mm distance
        }
        else
        {
            Serial2.println(F("Unknown command!"));
        }
    }
}

void MainFunction::TMC_DiagHandler_Jatayu()
{
    // Jatayu.DiagHandler(); // Call the diagnostic handler for Jatayu
    isJatayuStalled = !isJatayuStalled;
    Serial2.println(F("Jatayu Stalled!!"));
    Jatayu.Spin_Steps(1, Jatayu.getCurrentDir());
}

void MainFunction::TMC_DiagHandler_RahwanaSita()
{
    // RahwanaSita.DiagHandler(); // Call the diagnostic handler for Rahwana and Sita
    isRahwanaSitaStalled = !isRahwanaSitaStalled;
    Serial2.println(F("RahwanaSita Stalled!!"));
    RahwanaSita.Spin_Steps(1, RahwanaSita.getCurrentDir());
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
    Jatayu.DefaultPosition();
    RahwanaSita.DefaultPosition();

    /* Measure each for calibration */
    Jatayu.MeasureMovement();
    RahwanaSita.MeasureMovement();

    /* Homing for default */
    Jatayu.DefaultPosition();
    RahwanaSita.DefaultPosition();
}

void MainFunction::Calibration::Wayang_Servo()
{
}
