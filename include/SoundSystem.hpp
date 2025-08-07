#ifndef INC_SOUNDSYSTEM_HPP
#define INC_SOUNDSYSTEM_HPP

#include <Arduino.h>
#include <stdint.h>
#include <DFRobotDFPlayerMini.h>

#define USART_PORT_FOR_SOUNDSYSTEM Serial6 // Define the USART port for DFPlayer Mini communication

enum WHAT_AUDIO_FOLDER
{
    THE_SHOW_FOLDER = 1, // Folder where the show audio files are stored
    SYSTEM_FOLDER = 2,   // Folder for system sounds
};

enum SYSTEM_AUDIO
{
    SYSTEM_STARTS = 1,
    CALIBRATION_DONE = 2,
    SHOW_STARTS = 3,
    SHOW_IS_FINISHED = 4,
    SHOW_IS_ABORTED = 5,
    SHOW_IS_PAUSED = 6,
    SHOW_IS_RESUMED = 7,
    BACKGROUND_MUSIC = 8,
};

enum SHOW_AUDIO
{
    SITA_DIALOGUE = 1,
    JATAYU_DIALOGUE = 2,
    RAHWANA_DIALOGUE = 3,
    RAHWANA_ATTACK = 4,
    RAHWANA_HURT_1 = 5,
    RAHWANA_HURT_2 = 6,
    RAHWANA_HURT_3 = 7,
    RAHWANA_TAUNT = 8,
};

namespace SoundSystem
{
    void Init();
    void PlayAudio(uint8_t folder, uint8_t file);
}

#endif // INC_SOUNDSYSTEM_HPP