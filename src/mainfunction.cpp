#include "main.hpp"
#include "TMC2209.hpp"

TMCWayang Jatayu("right");
TMCWayang RahwanaSita("left");

void MainFunction::System_Setup()
{
    Jatayu.Init();                                                        // Initialize the TMC2209 driver for Jatayu
    attachInterrupt(Jatayu.getDiagPin(), TMC_DiagHandler_Jatayu, RISING); // Attach interrupt for stall detection

    RahwanaSita.Init();                                                             // Initialize the TMC2209 driver for Rahwana and Sita
    attachInterrupt(RahwanaSita.getDiagPin(), TMC_DiagHandler_RahwanaSita, RISING); // Attach interrupt for stall detection
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
        }
    }
}

void MainFunction::TMC_DiagHandler_Jatayu()
{
    Jatayu.DiagHandler(); // Call the diagnostic handler for Jatayu
}

void MainFunction::TMC_DiagHandler_RahwanaSita()
{
    RahwanaSita.DiagHandler(); // Call the diagnostic handler for Rahwana and Sita
}