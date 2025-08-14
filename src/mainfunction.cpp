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

bool isAlreadyPlayed = false;
bool isJatayuStalled = false;
bool isRahwanaSitaStalled = false;

TaskHandle_t USARTCommTask_Handler, PlayTask_Handler;
TaskHandle_t RFPHandler[4];
TaskHandle_t JFPHandler[6];

void MainFunction::System_Setup()
{
    /* Sound System */
    SoundSystem::Init();

    /* Wayang TMC */
    Jatayu_Horizontal.Init();      // Initialize the TMC2209 driver for Jatayu
    delay(100);                    // Allow some time for the driver to initialize
    RahwanaSita_Horizontal.Init(); // Initialize the TMC2209 driver for Rahwana and Sita
    delay(100);                    // Allow some time for the driver to initialize

    // RahwanaSita.RahwanaDirectControl(4, 90, 200); // Set Rahwana's left hand to the default position

    Serial2.println(F("System setup all done!!"));
    SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::SYSTEM_FOLDER, SYSTEM_AUDIO::SYSTEM_STARTS); // Play system start sound
    delay(2000);
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

    /* Create RFP Tasks */
    xTaskCreate(RFP1_Task, "RFP1_Task", 1024, NULL, 2, &RFPHandler[0]);
    xTaskCreate(RFP3_Task, "RFP3_Task", 1024, NULL, 2, &RFPHandler[1]);
    xTaskCreate(RFP3_AntiTask, "RFP3_AntiTask", 1024, NULL, 2, &RFPHandler[2]);
    xTaskCreate(RFFP_Task, "RFFP_Task", 1024, NULL, 2, &RFPHandler[3]);

    /* Create JFP Tasks */
    xTaskCreate(JFP1_Task, "JFP1_Task", 1024, NULL, 2, &JFPHandler[0]);
    xTaskCreate(JFP1_SubTask, "JFP1_SubTask", 1024, NULL, 2, &JFPHandler[1]);
    xTaskCreate(JFP3_Task, "JFP3_Task", 1024, NULL, 2, &JFPHandler[2]);
    xTaskCreate(JFP3_AntiTask, "JFP3_AntiTask", 1024, NULL, 2, &JFPHandler[3]);
    xTaskCreate(JFFP_Task, "JFFP_Task", 1024, NULL, 2, &JFPHandler[4]);
    xTaskCreate(JFFP_AntiTask, "JFFP_AntiTask", 1024, NULL, 2, &JFPHandler[5]);

    vTaskStartScheduler();
}

static void RFP1_Task(void *pvParam)
{
    Serial2.println(F("RFP1_Task created!"));
    vTaskSuspend(NULL);
    while (1)
    {
        RahwanaSita.RahwanaPointToFront();
        vTaskSuspend(NULL);
    }
}

static void RFP3_Task(void *pvParam)
{
    Serial2.println(F("RFP3_Task created!"));
    vTaskSuspend(NULL);
    while (1)
    {
        RahwanaSita.flick();
        vTaskSuspend(NULL);
    }
}

static void RFP3_AntiTask(void *pvParam)
{
    Serial2.println(F("RFP3_AntiTask created!"));
    vTaskSuspend(NULL);
    while (1)
    {
        RahwanaSita.defaultFaceOrientation();
        vTaskSuspend(NULL); // Delete the subtask after resetting face orientation
    }
}

static void RFFP_Task(void *pvParam)
{
    Serial2.println(F("RFFP_Task created!"));
    vTaskSuspend(NULL);
    while (1)
    {
        RahwanaSita.RahwanaDownFront();
        RahwanaSita.RahwanaMiddleFront();
        vTaskSuspend(NULL);
    }
}

static void JFP1_Task(void *pvParam)
{
    Serial2.println(F("JFP1_Task created!"));
    vTaskSuspend(NULL);
    while (1)
    {
        Jatayu.JatayuDirectControl(1, 0, 700); // Jatayu moves to the front position
        vTaskSuspend(NULL);                    // Delete the subtask after moving
    }
}

static void JFP1_SubTask(void *pvParam)
{
    Serial2.println(F("JFP1_SubTask created!"));
    vTaskSuspend(NULL); // Suspend the task until it is resumed by the main task
    while (1)
    {
        Jatayu_Horizontal.goToWhatPosition(190.0); // Jatayu moves to the front position
        if (PlayTask_Handler != NULL)
        {
            vTaskResume(PlayTask_Handler); // Suspend the play task while moving
        }
        if (USARTCommTask_Handler != NULL)
        {
            vTaskResume(USARTCommTask_Handler); // Resume USART task if it exists
        }
        vTaskSuspend(NULL); // Delete the subtask after moving
    }
}

// static void JFP2_Task(void *pvParam)
// {
//     while (1)
//     {
//         Jatayu.JatayuDirectControl(3, 180, 5500); // Jatayu moves to the default position
//         vTaskDelete(NULL);                        // Delete the subtask after moving
//     }
// }

// static void JFP2_SubTask(void *pvParam)
// {
//     while (1)
//     {
//         Jatayu_Horizontal.goToWhatPosition(600.0);
//         if (PlayTask_Handler != NULL)
//         {
//             vTaskResume(PlayTask_Handler); // Resume play task if it exists
//         }
//         if (USARTCommTask_Handler != NULL)
//         {
//             vTaskResume(USARTCommTask_Handler); // Resume USART task if it exists
//         }

//         vTaskDelete(NULL); // Delete the subtask after moving
//     }
// }

static void JFP3_Task(void *pvParam)
{
    Serial2.println(F("JFP3_Task created!"));
    vTaskSuspend(NULL); // Suspend the task until it is resumed by the main task
    while (1)
    {
        Jatayu.defaultFaceOrientation(); // Jatayu moves to the default face orientation
        Jatayu_Horizontal.goToWhatPosition(340.0);
        if (PlayTask_Handler != NULL)
        {
            vTaskResume(PlayTask_Handler); // Resume play task if it exists
        }
        if (USARTCommTask_Handler != NULL)
        {
            vTaskResume(USARTCommTask_Handler); // Resume USART task if it exists
        }
        vTaskSuspend(NULL); // Delete the subtask after moving
    }
}

static void JFP3_AntiTask(void *pvParam)
{
    Serial2.println(F("JFP3_AntiTask created!"));
    vTaskSuspend(NULL); // Suspend the task until it is resumed by the main task
    while (1)
    {
        Jatayu.flick(); // Jatayu moves to the default face orientation
        Jatayu_Horizontal.goToWhatPosition(600.0);
        if (PlayTask_Handler != NULL)
        {
            vTaskResume(PlayTask_Handler); // Resume play task if it exists
        }
        if (USARTCommTask_Handler != NULL)
        {
            vTaskResume(USARTCommTask_Handler); // Resume USART task if it exists
        }
        vTaskSuspend(NULL); // Delete the subtask after flicking
    }
}

static void JFFP_Task(void *pvParam)
{
    Serial2.println(F("JFFP_Task created!"));
    vTaskSuspend(NULL); // Suspend the task until it is resumed by the main task
    while (1)
    {
        Jatayu.JatayuDirectControl(1, 0, 700);     // Jatayu moves to the front position
        Jatayu_Horizontal.goToWhatPosition(540.0); // Jatayu moves to the front position
        if (PlayTask_Handler != NULL)
        {
            vTaskResume(PlayTask_Handler); // Resume play task if it exists
        }
        if (USARTCommTask_Handler != NULL)
        {
            vTaskResume(USARTCommTask_Handler); // Resume USART task if it exists
        }
        vTaskSuspend(NULL); // Delete the subtask after moving
    }
}

static void JFFP_AntiTask(void *pvParam)
{
    Serial2.println(F("JFFP_AntiTask created!"));
    vTaskSuspend(NULL); // Suspend the task until it is resumed by the main task
    while (1)
    {
        Jatayu_Horizontal.goToWhatPosition(0.0);
        if (PlayTask_Handler != NULL)
        {
            vTaskResume(PlayTask_Handler); // Resume play task if it exists
        }
        if (USARTCommTask_Handler != NULL)
        {
            vTaskResume(USARTCommTask_Handler); // Resume USART task if it exists
        }
        vTaskSuspend(NULL); // Delete the subtask after moving
    }
}

static void USART_Comm_Task(void *pvParam)
{
    UNUSED(pvParam);
    Serial2.println(F("Hi, I'm Mini MaYA 2025 USART Handler"));
    while (1)
    {
        String command = Serial2.readStringUntil('\n');
        if (command == "PlayTheShow")
        {
            Serial2.println("PlayTheShow Stack size: " + String(uxTaskGetStackHighWaterMark(PlayTask_Handler)));
            if (isAlreadyPlayed)
            {
                Serial2.println(F("The show is already playing!!"));
                vTaskResume(PlayTask_Handler); // instead of using creating method, we resume the task
            }
            else if (PlayTask_Handler == NULL)
            {
                Serial2.println(F("PlayTheShow Task created!!"));
                /* Create play task */
                xTaskCreate(
                    Play_Task,
                    "Play_Task",
                    PLAY_TASK_HEAP,
                    NULL,
                    2,
                    &PlayTask_Handler);
            }
            else
            {
                vTaskResume(PlayTask_Handler);
                SoundSystem::ResumeAudio();
            }
        }
        else if (command == "PauseTheShow")
        {
            Serial2.println(F("The show is paused!!"));
            /* Pause the play task */
            SoundSystem::PauseAudio();
            if (PlayTask_Handler != NULL)
            {
                vTaskSuspend(PlayTask_Handler);
            }
        }
        else if (command == "AbortTheShow")
        {
            Serial2.println(F("The show is aborted!!"));
            /* Kill the play task */
            SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::SYSTEM_FOLDER, SYSTEM_AUDIO::SHOW_IS_ABORTED);
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
            Jatayu_Horizontal.goToWhatPosition(100.0);
        }
        else if (command == "homing1")
        {
            Serial2.println(F("homing1 command"));
            Jatayu_Horizontal.DefaultPosition();
        }
        else if (command == "move2")
        {
            // int hajime = 0;
            // int sutopu = 0;
            Serial2.println(F("move2 command"));
            // hajime = millis();
            RahwanaSita_Horizontal.goToWhatPosition(100.0);
            // sutopu = millis() - hajime; // time taken to move
            // Serial2.println("Constant speed: " + String(100.0 / ((float)sutopu / 1000.0)) + " mm/s");
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
        else if (command == "JDef")
        {
            Jatayu.defaultFaceOrientation();
            Jatayu.JatayuDirectControl(1, 90, 1000); // Jatayu moves to the default position
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
        else if (command == "scene1")
        {
            //     // MainFunction::Calibration::Wayang_Servo();

            //     SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::SYSTEM_FOLDER, SYSTEM_AUDIO::BACKGROUND_MUSIC);
            //     Jatayu_Horizontal.goToWhatPosition(280.0);
            //     RahwanaSita_Horizontal.goToWhatPosition(280.0);

            //     vTaskDelay(1000 / portTICK_PERIOD_MS);
            //     RahwanaSita_Horizontal.goToWhatPosition(500.0);
            //     SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::RAHWANA_ATTACK);
            //     // (783) Rahwana_Attack
            //     vTaskDelay(283 / portTICK_PERIOD_MS);
            //     xTaskCreate(RFP1_Task, "RFP1_Task", 2048, NULL, 1, &RahwanaFightPhase_Handler);
            //     xTaskCreate(JFP1_Task, "JFP1_Task", 2048, NULL, 1, &JatayuFightPhase_Handler);
            //     xTaskCreate(JFP1_SubTask, "JFP1_SubTask", 2048, NULL, 1, &JatayuFightPhase_SubHandler);
            //     vTaskSuspend(USARTCommTask_Handler); // Suspend the play task while moving

            //     SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::SYSTEM_FOLDER, SYSTEM_AUDIO::BACKGROUND_MUSIC);
            //     RahwanaSita.RahwanaDownFront();
            //     Jatayu_Horizontal.goToWhatPosition(600.0);
            //     Jatayu.flick();

            //     SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::RAHWANA_HURT_1);
            //     vTaskDelay(237 / portTICK_PERIOD_MS); // (937) Rahwana_Hurt_1
            //     Jatayu.JatayuAttack();

            //     RahwanaSita_Horizontal.goToWhatPosition(520.0);
            //     RahwanaSita_Horizontal.goToWhatPosition(500.0);

            //     SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::RAHWANA_HURT_2);
            //     vTaskDelay(204 / portTICK_PERIOD_MS); // (904) Rahwana_Hurt_2
            //     Jatayu.JatayuAttack();

            //     RahwanaSita_Horizontal.goToWhatPosition(520.0);
            //     RahwanaSita_Horizontal.goToWhatPosition(500.0);

            //     SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::RAHWANA_HURT_3);
            //     Jatayu.JatayuAttack();

            //     RahwanaSita_Horizontal.goToWhatPosition(520.0);
            //     RahwanaSita_Horizontal.goToWhatPosition(500.0);

            //     SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::SYSTEM_FOLDER, SYSTEM_AUDIO::BACKGROUND_MUSIC);

            //     xTaskCreate(RFP3_Task, "RFP3_Task", 1024, NULL, 1, &RahwanaFightPhase_SubHandler);
            //     xTaskCreate(JFP3_Task, "JFP3_Task", 1024, NULL, 1, &JatayuFightPhase_SubHandler);
            //     vTaskSuspend(USARTCommTask_Handler);

            //     xTaskCreate(RFP3_AntiTask, "RFP3_AntiTask", 1024, NULL, 1, &RahwanaFightPhase_SubHandler);
            //     xTaskCreate(JFP3_AntiTask, "JFP3_AntiTask", 1024, NULL, 1, &JatayuFightPhase_SubHandler);
            //     vTaskSuspend(USARTCommTask_Handler);

            //     xTaskCreate(RFP3_Task, "RFP3_Task", 1024, NULL, 1, &RahwanaFightPhase_SubHandler);
            //     xTaskCreate(JFP3_Task, "JFP3_Task", 1024, NULL, 1, &JatayuFightPhase_SubHandler);
            //     vTaskSuspend(USARTCommTask_Handler);

            //     Jatayu_Horizontal.goToWhatPosition(150.0);

            //     Jatayu.JatayuDirectControl(1, 110, 350);
            //     Jatayu.JatayuDirectControl(2, 90, 350);

            //     RahwanaSita.defaultFaceOrientation();
            //     RahwanaSita_Horizontal.goToWhatPosition(300.0);
            //     SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::RAHWANA_TAUNT);
            //     // (1018) you pathetic avian !
            //     vTaskDelay(1018 / portTICK_PERIOD_MS);
            //     RahwanaSita.RahwanaPointToFront();
            //     vTaskDelay(3477 - 700 / portTICK_PERIOD_MS);

            //     // (3477)get back here !
            //     RahwanaSita.RahwanaLowPointToFront();
            //     vTaskDelay(500 / portTICK_PERIOD_MS);
            //     RahwanaSita.RahwanaDownFront();

            //     SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::SYSTEM_FOLDER, SYSTEM_AUDIO::BACKGROUND_MUSIC);
            //     Jatayu_Horizontal.goToWhatPosition(520.0);

            //     xTaskCreate(JFFP_Task, "JFFP_Task", 1024, NULL, 1, &JatayuFightPhase_Handler);
            //     xTaskCreate(RFFP_Task, "RFFP_Task", 1024, NULL, 1, &RahwanaFightPhase_Handler);

            //     vTaskSuspend(USARTCommTask_Handler); // Suspend the USART task while moving

            //     RahwanaSita.RahwanaPointToFront();

            //     Jatayu_Horizontal.goToWhatPosition(0.0);

            //     RahwanaSita.RahwanaDownFront();
            //     RahwanaSita_Horizontal.goToWhatPosition(0.0);

            //     Serial2.println(F("Task done!!"));
            //     SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::SYSTEM_FOLDER, SYSTEM_AUDIO::SHOW_IS_FINISHED);
            //     vTaskDelay(2000 / portTICK_PERIOD_MS);
            // }
        }
    }
}

static void Play_Task(void *pvParam)
{
    UNUSED(pvParam);

    while (1)
    {
        isAlreadyPlayed = false; // Reset the flag to indicate the show is not started yet
        Serial2.println(F("Playing the show!!"));
        MainFunction::Calibration::Wayang_Servo();
        SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::SYSTEM_FOLDER, SYSTEM_AUDIO::SHOW_STARTS);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        // // /* The show begin! */

        // RahwanaSita.flick();
        // SoundSystem::PlayBackgroundMusic();
        // RahwanaSita_Horizontal.goToWhatPosition(280.0);

        // /* Sita */
        // SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::SITA_DIALOGUE);

        // // (1272) (sobbing)
        // vTaskDelay(1272 / portTICK_PERIOD_MS);
        // RahwanaSita.SitaPointToSelf();
        // vTaskDelay(1977 - 900 / portTICK_PERIOD_MS);

        // // (1977) O Laksmana,
        // RahwanaSita.SitaDownFront();
        // RahwanaSita.SitaMiddleFront();
        // vTaskDelay(3591 - 1977 - 700 - 700 / portTICK_PERIOD_MS);

        // // (3591) Forgive me (4453) and do not be angry with me. (6034)
        // RahwanaSita.SitaPointToFront();
        // vTaskDelay(4453 - 3591 - 700 / portTICK_PERIOD_MS);

        // RahwanaSita.SitaDownFront();
        // RahwanaSita.SitaMiddleFront();
        // vTaskDelay(6034 - 4453 - 700 - 700 / portTICK_PERIOD_MS);

        // // (6544) I am the sinful one,
        // RahwanaSita.SitaDownFront();
        // RahwanaSita.SitaPointToSelf();
        // vTaskDelay(8144 - 6544 - 700 - 900 / portTICK_PERIOD_MS);

        // // (8125) i did not realize that you disobey my order out of love. (11554)
        // RahwanaSita.SitaDownFront();
        // RahwanaSita.SitaPointToFront();
        // vTaskDelay(11554 - 8144 - 700 - 700 - 700 / portTICK_PERIOD_MS);
        // RahwanaSita.SitaDownFront();

        // // (12135) How constant (13231) is the mind of a noble- minded man,
        // vTaskDelay(12135 - 11554 / portTICK_PERIOD_MS);
        // RahwanaSita.SitaMiddleFront();
        // vTaskDelay(15579 - 12135 - 700 / portTICK_PERIOD_MS);

        // // (15579) when he receives rude words,
        // RahwanaSita.SitaLowPointToFront();
        // vTaskDelay(17570 - 15579 - 700 / portTICK_PERIOD_MS);

        // // (17570) he just leaves.
        // RahwanaSita.SitaMiddleFront();
        // vTaskDelay(18832 - 17570 - 700 / portTICK_PERIOD_MS);

        // // (18832) Therefore you were right,
        // RahwanaSita.SitaPointToFront();
        // vTaskDelay(20289 - 18832 - 700 / portTICK_PERIOD_MS);

        // // (20289) not wrong.
        // RahwanaSita.SitaLowPointToFront();
        // vTaskDelay(21442 - 20289 - 700 / portTICK_PERIOD_MS);

        // // (21442) Because of my harsh words you left me.
        // RahwanaSita.SitaDownFront();
        // RahwanaSita.SitaPointToSelf();
        // vTaskDelay(24336 - 21442 - 700 - 900 / portTICK_PERIOD_MS);

        // // (24336) But a noble-minded person,
        // RahwanaSita.SitaDownFront();
        // RahwanaSita.SitaMiddleFront();
        // vTaskDelay(26128 - 24336 - 700 - 700 / portTICK_PERIOD_MS);

        // // (26128) given harsh words,
        // RahwanaSita.SitaPointToFront();
        // vTaskDelay(27547 - 26128 - 700 / portTICK_PERIOD_MS);

        // // (27547) is a little angry only for a while.
        // RahwanaSita.SitaLowPointToFront();
        // vTaskDelay(30114 - 27547 - 700 / portTICK_PERIOD_MS);

        // // (30114) If he sees,
        // RahwanaSita.SitaMiddleFront();
        // vTaskDelay(31310 - 30114 - 700 / portTICK_PERIOD_MS);

        // // (31310) that the evil person who says harsh words
        // RahwanaSita.SitaDownFront();
        // RahwanaSita.SitaPointToFront();
        // vTaskDelay(34182 - 31310 - 700 - 700 / portTICK_PERIOD_MS);

        // // (34182) meets misfortune and is miserable,
        // RahwanaSita.SitaDownFront();
        // RahwanaSita.SitaMiddleFront();
        // vTaskDelay(37087 - 34182 - 700 - 700 / portTICK_PERIOD_MS);

        // // (37087) he will come forward to help him out of compassion.
        // RahwanaSita.SitaDownFront();
        // RahwanaSita.SitaPointToFront();
        // vTaskDelay(40640 - 37087 - 700 - 700 - 700 / portTICK_PERIOD_MS);
        // RahwanaSita.SitaDownFront();

        // // (40640) Therefore, O, Laksmana,
        // RahwanaSita.SitaPointToFront();
        // vTaskDelay(42474 - 40640 - 700 / portTICK_PERIOD_MS);

        // // (42474) noble and gentle brother,
        // RahwanaSita.SitaLowPointToFront();
        // vTaskDelay(44855 - 42474 - 700 / portTICK_PERIOD_MS);

        // // (44855) come and save me from great distress.
        // RahwanaSita.SitaPointToFront();
        // vTaskDelay(47655 - 44855 - 700 / portTICK_PERIOD_MS);

        // // (47655) Your curse has come true,
        // RahwanaSita.SitaDownFront();
        // RahwanaSita.SitaMiddleFront();
        // vTaskDelay(49775 - 47655 - 700 - 700 / portTICK_PERIOD_MS);

        // // (49775) now I am captured by the enemy.
        // RahwanaSita.SitaDownFront();
        // RahwanaSita.SitaPointToSelf();
        // vTaskDelay(1000 / portTICK_PERIOD_MS);

        // // Back to position then switch to Rahwana
        // SoundSystem::PlayBackgroundMusic();
        // RahwanaSita_Horizontal.goToWhatPosition(0.0);

        // RahwanaSita.defaultFaceOrientation();
        // RahwanaSita_Horizontal.goToWhatPosition(280.0);
        // /* Jatayu */

        // Jatayu_Horizontal.goToWhatPosition(280.0);

        // // *Garuda sounds*
        // SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::JATAYU_DIALOGUE);
        // vTaskDelay(1159 / portTICK_PERIOD_MS);
        // // (1159) O, Dasanana,
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkBob();
        // vTaskDelay(2774 - 1159 - 700 - 700 / portTICK_PERIOD_MS);

        // // (2774) the evil and savage of the ascetics,
        // Jatayu.JatayuTalkSide();
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkSide();
        // Jatayu.JatayuTalkBob();
        // vTaskDelay(6026 - 2774 - 700 - 700 - 700 - 700 / portTICK_PERIOD_MS);

        // // (6026) because you have the heart to insult and humiliate priests.
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkBob();
        // vTaskDelay(11000 - 6026 - 700 - 700 - 700 - 700 / portTICK_PERIOD_MS);

        // // (11000) How debased and depraved are you!
        // Jatayu.JatayuTalkSide();
        // Jatayu.JatayuTalkSide();
        // vTaskDelay(13880 - 11000 - 700 - 700 / portTICK_PERIOD_MS);

        // // (13880) Come on!
        // Jatayu.JatayuDirectControl(1, 0, 700);
        // vTaskDelay(14729 - 13880 - 700 / portTICK_PERIOD_MS);

        // // (14729) Attack me if you are really a hero in battle.
        // Jatayu.JatayuDirectControl(1, 65, 1000);
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkBob();
        // vTaskDelay(18752 - 14729 - 1000 - 700 - 700 / portTICK_PERIOD_MS);

        // // (18752) Although Raghawa is merciful towards the hermits,
        // Jatayu.JatayuTalkSide();
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkSide();
        // Jatayu.JatayuTalkBob();
        // vTaskDelay(22049 - 18752 - 700 - 700 - 700 - 700 / portTICK_PERIOD_MS);

        // // (22049) and truthful and devoted to his father,
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkBob();
        // vTaskDelay(25301 - 22049 - 700 - 700 - 700 - 700 / portTICK_PERIOD_MS);

        // // (25301) you have the heart to render violence upon him,
        // Jatayu.JatayuTalkSide();
        // Jatayu.JatayuTalkSide();
        // Jatayu.JatayuTalkSide();
        // vTaskDelay(28705 - 25301 - 700 - 700 - 700 / portTICK_PERIOD_MS);

        // // (28705) o, debased evildoer!
        // Jatayu.JatayuDirectControl(1, 0, 700);
        // vTaskDelay(31208 - 28705 - 700 / portTICK_PERIOD_MS);

        // // (31208) How despicable you are,
        // Jatayu.JatayuDirectControl(1, 65, 1000);
        // vTaskDelay(33656 - 31208 - 1000 / portTICK_PERIOD_MS);

        // // (33656) torturing the noble people.
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkBob();
        // vTaskDelay(36761 - 33656 - 700 - 700 - 700 / portTICK_PERIOD_MS);

        // // (36761) This is Jatayu.
        // Jatayu.JatayuDirectControl(1, 0, 700);
        // vTaskDelay(38258 - 36761 - 700 / portTICK_PERIOD_MS);

        // // (38258) I will destroy the outrageous ones (41161) as the Sungod destroys darkness.
        // Jatayu.JatayuDirectControl(1, 45, 700);
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkSide();
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkSide();
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkSide();
        // vTaskDelay(44526 - 38528 - 700 * 7 / portTICK_PERIOD_MS);

        // // (44526) I am the humble servant of Rama.
        // Jatayu.JatayuDirectControl(1, 65, 1000);
        // vTaskDelay(47361 - 44526 - 1000 / portTICK_PERIOD_MS);

        // // (47361) It is my obligation to protect Rama
        // Jatayu.JatayuDirectControl(1, 45, 700);
        // vTaskDelay(50214 - 47361 - 700 / portTICK_PERIOD_MS);

        // // (50214) as a friend of his late father.
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkBob();
        // Jatayu.JatayuTalkBob();
        // vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Test
        SoundSystem::PlayBackgroundMusic();
        Jatayu_Horizontal.goToWhatPosition(280.0);
        RahwanaSita_Horizontal.goToWhatPosition(280.0);
        Jatayu.JatayuTakeoff();
        Jatayu.JatayuDirectControl(1, 110, 1000); // Jatayu moves to the default position

        /* Rahwana */
        SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::RAHWANA_DIALOGUE);
        // (526) You Insolent bastard!
        vTaskDelay(526 / portTICK_PERIOD_MS);
        RahwanaSita.RahwanaPointToFront();
        vTaskDelay(3084 - 526 / portTICK_PERIOD_MS);

        // (3084) Have at you!
        RahwanaSita.RahwanaDownFront();
        RahwanaSita.RahwanaPointToFront();
        RahwanaSita.RahwanaDownFront();

        // *battle phase*

        /*Phase 1*/
        // Rahwana attacks, but misses Jatayu.
        // How:
        // Rahwana approaches Jatayu
        // Rahwana attacks using ptf but mid-swing Jatayu dodges by backing up (just have them move at the same time immediately)

        /*Phase 2*/
        // Jatayu repositions behind Rahwana and attacks him by pecking. (use JatayuAttack())
        // How:
        // After previous action is done, Jatayu moves to behind rahwana, spins, and attack rahwana's back using jatayuattack [illus_1]

        /*Phase 3*/
        // Rahwana attemps to chase Jatayu around the stage but is always one step late
        // How:
        // Rahwana turns around while Jatayu flies across him, jatayu turns around
        // repeat this for 2 more times
        // Jatayu moves a larger distance away from rahwana
        // Rahwana turns around and finally faces jatayu
        // Rahwana taunts

        /*Final Phase*/
        // Jatayu uses talon on Rahwana, but this time Rahwana can retaliate
        // How:
        // Rahwana blocks the attack
        // Jatayu falls down and moves away at the lowest height

        // Battle ends with Rahwana walking off-stage.

        /* Other voice clips reference: */
        SoundSystem::PlayBackgroundMusic();
        RahwanaSita_Horizontal.goToWhatPosition(500.0);
        SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::RAHWANA_ATTACK);
        // (783) Rahwana_Attack
        vTaskDelay(283 / portTICK_PERIOD_MS);

        // xTaskCreate(RFP1_Task, "RFP1_Task", 1024, NULL, 2, NULL);
        vTaskResume(RFPHandler[0]);
        // xTaskCreate(JFP1_Task, "JFP1_Task", 1024, NULL, 2, NULL);
        vTaskResume(JFPHandler[0]);
        // xTaskCreate(JFP1_SubTask, "JFP1_SubTask", 1024, NULL, 2, NULL);
        vTaskResume(JFPHandler[1]);

        vTaskSuspend(PlayTask_Handler); // Suspend the play task while moving
        vTaskSuspend(USARTCommTask_Handler);

        SoundSystem::PlayBackgroundMusic();
        RahwanaSita.RahwanaDownFront();
        Jatayu_Horizontal.goToWhatPosition(600.0);
        Jatayu.flick();

        SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::RAHWANA_HURT_1);
        vTaskDelay(237 / portTICK_PERIOD_MS); // (937) Rahwana_Hurt_1
        Jatayu.JatayuAttack();

        RahwanaSita_Horizontal.goToWhatPosition(520.0);
        RahwanaSita_Horizontal.goToWhatPosition(500.0);

        SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::RAHWANA_HURT_2);
        vTaskDelay(204 / portTICK_PERIOD_MS); // (904) Rahwana_Hurt_2
        Jatayu.JatayuAttack();

        RahwanaSita_Horizontal.goToWhatPosition(520.0);
        RahwanaSita_Horizontal.goToWhatPosition(500.0);

        SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::RAHWANA_HURT_3);
        Jatayu.JatayuAttack();

        RahwanaSita_Horizontal.goToWhatPosition(520.0);
        RahwanaSita_Horizontal.goToWhatPosition(500.0);

        SoundSystem::PlayBackgroundMusic();

        // xTaskCreate(RFP3_Task, "RFP3_Task", 1024, NULL, 2, NULL);
        vTaskResume(RFPHandler[1]);
        // xTaskCreate(JFP3_Task, "JFP3_Task", 1024, NULL, 2, NULL);
        vTaskResume(JFPHandler[2]);

        // Suspend USART task as well to save clocks
        vTaskSuspend(PlayTask_Handler);
        vTaskSuspend(USARTCommTask_Handler);

        // xTaskCreate(RFP3_AntiTask, "RFP3_AntiTask", 1024, NULL, 2, NULL);
        vTaskResume(RFPHandler[2]);
        // xTaskCreate(JFP3_AntiTask, "JFP3_AntiTask", 1024, NULL, 2, NULL);
        vTaskResume(JFPHandler[3]);

        // Suspend USART task as well to save clocks
        vTaskSuspend(PlayTask_Handler);
        vTaskSuspend(USARTCommTask_Handler);

        // xTaskCreate(RFP3_Task, "RFP3_Task", 1024, NULL, 2, NULL);
        vTaskResume(RFPHandler[1]);
        // xTaskCreate(JFP3_Task, "JFP3_Task", 1024, NULL, 2, NULL);
        vTaskResume(JFPHandler[2]);

        // Suspend USART task as well to save clocks
        vTaskSuspend(PlayTask_Handler);
        vTaskSuspend(USARTCommTask_Handler);

        Jatayu_Horizontal.goToWhatPosition(150.0);
        Jatayu.JatayuDirectControl(1, 110, 200);
        RahwanaSita.defaultFaceOrientation();
        RahwanaSita_Horizontal.goToWhatPosition(300.0);
        SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::THE_SHOW_FOLDER, SHOW_AUDIO::RAHWANA_TAUNT);
        // (1018) you pathetic avian !
        vTaskDelay(1018 / portTICK_PERIOD_MS);
        RahwanaSita.RahwanaPointToFront();
        vTaskDelay(3477 - 700 / portTICK_PERIOD_MS);

        // (3477)get back here !
        RahwanaSita.RahwanaLowPointToFront();
        vTaskDelay(500 / portTICK_PERIOD_MS);
        RahwanaSita.RahwanaDownFront();

        SoundSystem::PlayBackgroundMusic();
        Jatayu_Horizontal.goToWhatPosition(520.0);

        // xTaskCreate(JFFP_Task, "JFFP_Task", 1024, NULL, 2, NULL);
        vTaskResume(JFPHandler[4]);
        // xTaskCreate(RFFP_Task, "RFFP_Task", 1024, NULL, 2, NULL);
        vTaskResume(RFPHandler[3]);

        // Suspend USART task as well to save clocks
        vTaskSuspend(PlayTask_Handler);
        vTaskSuspend(USARTCommTask_Handler);

        RahwanaSita_Horizontal.goToWhatPosition(360.0);
        RahwanaSita.RahwanaPointToFront();

        Jatayu_Horizontal.LeaveTheScene();

        RahwanaSita.RahwanaDownFront();
        RahwanaSita_Horizontal.LeaveTheScene();

        Jatayu.JatayuDirectControl(1, 110, 1000); // Jatayu moves to the default position

        /* End */

        /* for killing its own task */
        Serial2.println(F("Task done!!"));
        SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::SYSTEM_FOLDER, SYSTEM_AUDIO::SHOW_IS_FINISHED);
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        Serial2.println("Stack size: " + String(uxTaskGetStackHighWaterMark(NULL)));

        isAlreadyPlayed = true;
        vTaskSuspend(NULL); // Suspend itself to wait for the next command
    }
}

/* Calibration Feature */
// Just homing both wayang's gantry
void MainFunction::Calibration::VSlotCalibration()
{
    /* Homing both wayang */
    Jatayu_Horizontal.DefaultPosition();
    RahwanaSita_Horizontal.DefaultPosition();

    // /* Measure each for calibration */
    Jatayu_Horizontal.MeasureMovement();
    RahwanaSita_Horizontal.MeasureMovement();

    /* Homing for default */
    Jatayu_Horizontal.DefaultPosition();
    RahwanaSita_Horizontal.DefaultPosition();

    /* Testing */
    Jatayu_Horizontal.goToWhatPosition(200.0);
    Jatayu_Horizontal.goToWhatPosition(0.0);
    RahwanaSita_Horizontal.goToWhatPosition(200.0);
    RahwanaSita_Horizontal.goToWhatPosition(0.0);

    SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::SYSTEM_FOLDER, SYSTEM_AUDIO::CALIBRATION_DONE);
    delay(2000); // Wait for 2 seconds to let the sound play
}

void MainFunction::Calibration::Wayang_Servo()
{
    Jatayu_Horizontal.LeaveTheScene();
    Jatayu.flick();
    Jatayu.defaultFaceOrientation();
    Jatayu.JatayuDefaultFly();

    RahwanaSita_Horizontal.LeaveTheScene();
    RahwanaSita.flick();
    RahwanaSita.SitaDownFront();
    RahwanaSita.defaultFaceOrientation();
    RahwanaSita.RahwanaDownFront();

    SoundSystem::PlayAudio(WHAT_AUDIO_FOLDER::SYSTEM_FOLDER, SYSTEM_AUDIO::CALIBRATION_DONE);
    delay(2500); // Wait for 2.5 seconds to let the sound play
}
