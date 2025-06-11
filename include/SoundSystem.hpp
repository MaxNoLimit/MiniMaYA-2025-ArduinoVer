#ifndef INC_SOUNDSYSTEM_HPP
#define INC_SOUNDSYSTEM_HPP

#include <Arduino.h>
#include <stdint.h>
#include <DFRobotDFPlayerMini.h>

#define USART_PORT_FOR_SOUNDSYSTEM Serial6 // Define the USART port for DFPlayer Mini communication

namespace SoundSystem
{
    void Init();
    void PlayAudio(uint8_t folder, uint8_t file);
}

#endif // INC_SOUNDSYSTEM_HPP