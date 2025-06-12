#include "main.hpp"
#include "TMC2209.hpp"
#include "config.hpp"

TMCWayang Jatayu("right");
TMCWayang RahwanaSita("left");

bool isJatayuStalled = false;
bool isRahwanaSitaStalled = false;

void MainFunction::System_Setup()
{
    Jatayu.Init();      // Initialize the TMC2209 driver for Jatayu
    RahwanaSita.Init(); // Initialize the TMC2209 driver for Rahwana and Sita
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