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
    MainFunction::Calibration::Wayang_Servo();
    for (;;)
    {
        /* The show begin! */

        /* Sita */
        RahwanaSita.flick();
        RahwanaSita_Horizontal.WalkToScene();
        // (1272) (sobbing)

        // (1977) O Laksmana,

        // (3591) Forgive me (4453) and do not be angry with me. (6034)

        // (6544) I am the sinful one,

        // (8125) i did not realize that you disobey my order out of love. (11554)

        // (12135) How constant (13231) is the mind of a noble- minded man,

        // (15579) when he receives rude words,

        // (17570) he just leaves.

        // (18832) Therefore you were right,

        // (20289) not wrong.

        // (21442) Because of my harsh words you left me.

        // (24336) But a noble-minded person,

        // (26128) given harsh words,

        // (27547) is a little angry only for a while.

        // (30114) If he sees,

        // (31310) that the evil person who says harsh words

        // (34182) meets misfortune and is miserable,

        // (37087) he will come forward to help him out of compassion.

        // (40640) Therefore, O, Laksmana,

        // (42474) noble and gentle brother,

        // (44855) come and save me from great distress.

        // (47655) Your curse has come true,

        // (49775) now I am captured by the enemy.

        // Back to position then switch to Rahwana
        RahwanaSita_Horizontal.DefaultPosition();
        RahwanaSita.defaultFaceOrientation();
        RahwanaSita_Horizontal.WalkToScene();

        /* Jatayu */
        Jatayu_Horizontal.WalkToScene();

        // *Garuda sounds*

        // (1159) O, Dasanana,

        // (2774) the evil and savage of the ascetics,

        // (6026) because you have the heart to insult and humiliate priests.

        // (11000) How debased and depraved are you!

        // (13880) Come on!

        // (14729) Attack me if you are really a hero in battle.

        // (18752) Although Raghawa is merciful towards the hermits,

        // (22049) and truthful and devoted to his father,

        // (25301) you have the heart to render violence upon him,

        // (28705) o, debased evildoer!

        // (31208) How despicable you are,

        // (33656) torturing the noble people.

        // (36761) This is Jatayu.

        // (38258) I will destroy the outrageous ones (41161) as the Sungod destroys darkness.

        // (44526) I am the humble servant of Rama.

        // (47361) It is my obligation to protect Rama

        // (50214) as a friend of his late father.

        /* Rahwana */
        // (526) You Insolent bastard!

        // (3084) Have at you!

        // *battle phase*

        // Rahwana attacks, but misses Jatayu.

        // Jatayu repositions behind Rahwana and attacks him by pecking. (have to use direct control)

        // Rahwana attemps to chase Jatayu around the stage but is always one step late

        // Jatayu uses talon on Rahwana’s back, but this time Rahwana can retaliate

        // Battle ends with Rahwana walking off-stage.

        // Other voice clips reference:

        // (783) Rahwana_Attack_Normalized_Filtered

        // (937) Rahwana_Hurt1_Normalized_Filtered

        // (904) Rahwana_Hurt2_Normalized_Filtered

        // (663) Rahwana_Hurt3_Normalized_Filtered

        /* End */

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
    Jatayu_Horizontal.MeasureMovement();
    RahwanaSita_Horizontal.MeasureMovement();

    /* Homing for default */
    Jatayu_Horizontal.DefaultPosition();
    RahwanaSita_Horizontal.DefaultPosition();
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
