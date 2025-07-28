#include "main.hpp"
#include "TMC2209.hpp"
#include "config.hpp"
#include "SoundSystem.hpp"
#include <STM32FreeRTOS.h>
#include "Wayang.hpp"
#include <string.h>

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
            if (PlayTask_Handler == NULL)
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
            else
            {
                vTaskResume(PlayTask_Handler);
            }
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

            // /* not yet trying this method */

            // /* buffer to send */
            // uint8_t buffer_to_send[8] = "VSDone";

            // /* sending buffer with custom allocation */
            // Serial2.write(buffer_to_send, strlen((const char *)buffer_to_send));
            Serial2.write("VSDone");
            Serial2.write('\n');
        }
        else if (command == "WayangServo")
        {
            Serial2.println(F("WayangServo Task created!!"));
            /* Create wayangservo task */
            MainFunction::Calibration::Wayang_Servo();

            // /* not yet trying this method */

            // /* buffer to send */
            // uint8_t buffer_to_send[8] = "WSDone";

            // /* sending buffer with custom allocation */
            // Serial2.write(buffer_to_send, strlen((const char *)buffer_to_send));
            Serial2.write("WSDone");
            Serial2.write('\n');
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
        else if (command == "measure1")
        {
            Serial2.println(F("measure1 command"));
            Jatayu_Horizontal.MeasureMovement();
        }
        else if (command == "measure2")
        {
            Serial2.println(F("measure2 command"));
            RahwanaSita_Horizontal.MeasureMovement();
        }
        else if (command == "JF")
        {
            Serial2.println(F("Jatayu Default Fly command"));
            Jatayu.JatayuDefaultFly();
        }

        else if (command == "JTO")
        {
            Serial2.println(F("Jatayu Take Off command"));
            Jatayu.JatayuTakeoff();
        }
        else if (command == "JD")
        {
            Serial2.println(F("Jatayu Dive command"));
            Jatayu.JatayuDive();
        }
        else if (command == "JT")
        {
            Serial2.println(F("Jatayu Talon command"));
            Jatayu.JatayuTalon();
        }
        else if (command == "JFlick")
        {
            Serial2.println(F("Jatayu flick command"));
            Jatayu.flick();
        }
        else if (command == "RSFlick")
        {
            RahwanaSita.flick();
        }
        else if (command == "RSDef")
        {
            RahwanaSita.defaultFaceOrientation();
        }

        else if (command == "RPTF")
        {
            RahwanaSita.RahwanaPointToFront();
        }
        else if (command == "RLPTF")
        {
            RahwanaSita.RahwanaLowPointToFront();
        }
        else if (command == "RMF")
        {
            RahwanaSita.RahwanaMiddleFront();
        }
        else if (command == "RDF")
        {
            RahwanaSita.RahwanaDownFront();
        }
        else if (command == "RPTS")
        {
            RahwanaSita.RahwanaPointToSelf();
        }
        else if (command == "SPTF")
        {
            RahwanaSita.SitaPointToFront();
        }
        else if (command == "SLPTF")
        {
            RahwanaSita.SitaLowPointToFront();
        }
        else if (command == "SMF")
        {
            RahwanaSita.SitaMiddleFront();
        }
        else if (command == "SDF")
        {
            RahwanaSita.SitaDownFront();
        }
        else if (command == "SPTS")
        {
            RahwanaSita.SitaPointToSelf();
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
// Just homing both wayang's gantry
void MainFunction::Calibration::VSlotCalibration()
{
    /* Homing both wayang */
    Jatayu_Horizontal.DefaultPosition();
    RahwanaSita_Horizontal.DefaultPosition();

    /* Measure each for calibration */
    // Jatayu_Horizontal.MeasureMovement();
    // RahwanaSita_Horizontal.MeasureMovement();

    /* Homing for default */
    // Jatayu_Horizontal.DefaultPosition();
    // RahwanaSita_Horizontal.DefaultPosition();
}

void MainFunction::Calibration::Wayang_Servo()
{
    Jatayu_Horizontal.LeaveTheScene();
    Jatayu.JatayuDefaultFly();

    // not ready yet
    RahwanaSita_Horizontal.LeaveTheScene();
    RahwanaSita.flick();
    RahwanaSita.SitaDownFront();
    RahwanaSita.defaultFaceOrientation();
    RahwanaSita.RahwanaDownFront();
    // RahwanaSita.defaultHandPosition();
}
