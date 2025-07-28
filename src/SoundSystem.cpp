#include "SoundSystem.hpp"

DFRobotDFPlayerMini myDFPlayer;
HardwareSerial Serial6(PA12, PA11);

void SoundSystem::Init()
{
    USART_PORT_FOR_SOUNDSYSTEM.begin(9600); // Initialize Serial2 for DFPlayer Mini communication
    if (!myDFPlayer.begin(USART_PORT_FOR_SOUNDSYSTEM))
    {
        Serial2.println("DFPlayer Mini not found!");
        return;
    }
    Serial2.println(F("Sound System is online!!"));
    myDFPlayer.volume(20); // Set volume level (0-30)
}

void SoundSystem::PlayAudio(uint8_t folder, uint8_t file)
{
    if (myDFPlayer.available())
    {
        myDFPlayer.playFolder(folder, file); // Play audio from specified folder and file
        Serial2.printf("Playing audio from folder %d, file %d\n", folder, file);
    }
    else
    {
        Serial2.println("DFPlayer Mini is not available.");
    }
}