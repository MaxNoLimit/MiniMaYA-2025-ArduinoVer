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
    myDFPlayer.volume(25); // Set volume level (0-30)
}

void SoundSystem::PlayAudio(uint8_t folder, uint8_t file)
{
    myDFPlayer.playFolder(folder, file); // Play audio from specified folder and file
    Serial2.printf("Playing audio from folder %d, file %d\n", folder, file);
}

void SoundSystem::PauseAudio()
{
    myDFPlayer.pause(); // Pause the currently playing audio
    Serial2.println(F("Audio playback paused."));
}

void SoundSystem::ResumeAudio()
{
    myDFPlayer.start(); // Resume the paused audio
    Serial2.println(F("Audio playback resumed."));
}

void SoundSystem::PlayBackgroundMusic()
{
    Serial2.println(F("Background music started."));
    myDFPlayer.volume(18); // Set a lower volume for background music
    PlayAudio(WHAT_AUDIO_FOLDER::SYSTEM_FOLDER, SYSTEM_AUDIO::BACKGROUND_MUSIC);
}